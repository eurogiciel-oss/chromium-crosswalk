# Copyright 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import logging

from telemetry.core.backends.chrome_inspector import inspector_websocket
from telemetry.core.platform import tracing_options
from telemetry.timeline import trace_data as trace_data_module


class TracingUnsupportedException(Exception):
  pass


class TracingTimeoutException(Exception):
  pass

class TracingHasNotRunException(Exception):
  pass


class TracingBackend(object):
  def __init__(self, devtools_port):
    self._inspector_websocket = inspector_websocket.InspectorWebsocket(
        self._ErrorHandler)
    self._inspector_websocket.RegisterDomain(
        'Tracing', self._NotificationHandler)

    self._inspector_websocket.Connect(
        'ws://127.0.0.1:%i/devtools/browser' % devtools_port)
    self._trace_events = []
    self._is_tracing_running = False

  @property
  def is_tracing_running(self):
    return self._is_tracing_running

  def StartTracing(self, trace_options, custom_categories=None, timeout=10):
    """When first called, starts tracing, and returns True.

    If called during tracing, tracing is unchanged, and it returns False.
    """
    if self.is_tracing_running:
      return False
    # Reset collected tracing data from previous tracing calls.
    self._trace_events = []
    self._CheckNotificationSupported()
    # Map telemetry's tracing record_mode to the DevTools API string.
    # (The keys happen to be the same as the values.)
    m = {tracing_options.RECORD_UNTIL_FULL: 'record-until-full',
         tracing_options.RECORD_AS_MUCH_AS_POSSIBLE:
         'record-as-much-as-possible'}
    # DevTools started supporting RECORD_AS_MUCH_AS_POSSIBLE in Chrome 2118.
    # However, we send it for earlier versions as well because Chrome ignores
    # the unknown value and falls back to RECORD_UNTIL_FULL.
    req = {
        'method': 'Tracing.start',
        'params': {'options': m[trace_options.record_mode]}
        }
    if custom_categories:
      req['params']['categories'] = custom_categories
    self._inspector_websocket.SyncRequest(req, timeout)
    self._is_tracing_running = True
    return True

  def StopTracing(self, trace_data_builder, timeout=30):
    """Stops tracing and pushes results to the supplied TraceDataBuilder.

    If this is called after tracing has been stopped, trace data from the last
    tracing run is pushed.
    """
    if not self.is_tracing_running:
      if not self._trace_events:
        raise TracingHasNotRunException()
    else:
      req = {'method': 'Tracing.end'}
      self._inspector_websocket.SendAndIgnoreResponse(req)
      # After Tracing.end, chrome browser will send asynchronous notifications
      # containing trace data. This is until Tracing.tracingComplete is sent,
      # which means there is no trace buffers pending flush.
      try:
        self._inspector_websocket.DispatchNotificationsUntilDone(timeout)
      except \
          inspector_websocket.DispatchNotificationsUntilDoneTimeoutException \
          as err:
        raise TracingTimeoutException(
            'Trace data was not fully received due to timeout after %s '
            'seconds. If the trace data is big, you may want to increase the '
            'time out amount.' % err.elapsed_time)
    self._is_tracing_running = False
    trace_data_builder.AddEventsTo(
      trace_data_module.CHROME_TRACE_PART, self._trace_events)

  def _ErrorHandler(self, elapsed):
    logging.error('Unrecoverable error after %ds reading tracing response.',
                  elapsed)
    raise

  def _NotificationHandler(self, res):
    if 'Tracing.dataCollected' == res.get('method'):
      value = res.get('params', {}).get('value')
      if type(value) in [str, unicode]:
        self._trace_events.append(value)
      elif type(value) is list:
        self._trace_events.extend(value)
      else:
        logging.warning('Unexpected type in tracing data')
    elif 'Tracing.tracingComplete' == res.get('method'):
      return True

  def Close(self):
    self._inspector_websocket.Disconnect()

  def _CheckNotificationSupported(self):
    """Ensures we're running against a compatible version of chrome."""
    req = {'method': 'Tracing.hasCompleted'}
    res = self._inspector_websocket.SyncRequest(req)
    if res.get('response'):
      raise TracingUnsupportedException(
          'Tracing not supported for this browser')
    elif 'error' in res:
      return
