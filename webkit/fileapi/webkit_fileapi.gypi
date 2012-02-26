# Copyright (c) 2012 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'targets': [
    {
      'target_name': 'fileapi',
      'type': 'static_library',
      'variables': { 'enable_wexit_time_destructors': 1, },
      'dependencies': [
        '<(DEPTH)/base/base.gyp:base',
        '<(DEPTH)/net/net.gyp:net',
        '<(DEPTH)/third_party/leveldatabase/leveldatabase.gyp:leveldatabase',
        '<(DEPTH)/webkit/support/webkit_support.gyp:quota',
      ],
      'sources': [
        'file_system_callback_dispatcher.cc',
        'file_system_callback_dispatcher.h',
        'file_system_context.cc',
        'file_system_context.h',
        'file_system_dir_url_request_job.cc',
        'file_system_dir_url_request_job.h',
        'file_system_directory_database.cc',
        'file_system_directory_database.h',
        'file_system_file_util.cc',
        'file_system_file_util.h',
        'file_system_file_util_proxy.cc',
        'file_system_file_util_proxy.h',
        'file_system_mount_point_provider.h',
        'file_system_operation.cc',
        'file_system_operation.h',
        'file_system_operation_context.cc',
        'file_system_operation_context.h',
        'file_system_operation_interface.h',
        'file_system_options.cc',
        'file_system_options.h',
        'file_system_origin_database.cc',
        'file_system_origin_database.h',
        'file_system_path.cc',
        'file_system_path.h',
        'file_system_quota_client.cc',
        'file_system_quota_client.h',
        'file_system_quota_util.cc',
        'file_system_quota_util.h',
        'file_system_types.h',
        'file_system_url_request_job.cc',
        'file_system_url_request_job.h',
        'file_system_url_request_job_factory.cc',
        'file_system_url_request_job_factory.h',
        'file_system_usage_cache.cc',
        'file_system_usage_cache.h',
        'file_system_util.cc',
        'file_system_util.h',
        'file_writer_delegate.cc',
        'file_writer_delegate.h',
        'isolated_context.cc',
        'isolated_context.h',
        'local_file_util.cc',
        'local_file_util.h',
        'native_file_util.cc',
        'native_file_util.h',
        'obfuscated_file_util.cc',
        'obfuscated_file_util.h',
        'quota_file_util.cc',
        'quota_file_util.h',
        'sandbox_mount_point_provider.cc',
        'sandbox_mount_point_provider.h',
        'webfilewriter_base.cc',
        'webfilewriter_base.h',
      ],
      'conditions': [
        ['inside_chromium_build==0', {
          'dependencies': [
            '<(DEPTH)/webkit/support/setup_third_party.gyp:third_party_headers',
          ],
        }],
        ['chromeos==1', {
          'sources': [
            '../chromeos/fileapi/async_file_stream.h',
            '../chromeos/fileapi/cros_mount_point_provider.cc',
            '../chromeos/fileapi/cros_mount_point_provider.h',
            '../chromeos/fileapi/file_access_permissions.cc',
            '../chromeos/fileapi/file_access_permissions.h',
            '../chromeos/fileapi/file_util_async.h',
            '../chromeos/fileapi/remote_file_system_operation.cc',
            '../chromeos/fileapi/remote_file_system_operation.h',
            '../chromeos/fileapi/remote_file_system_proxy.h',
          ],
        }],
      ],
    },
  ],
}
