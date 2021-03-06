// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/browser_about_handler.h"

#include <string>

#include "base/bind.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/string_util.h"
#include "chrome/browser/lifetime/application_lifetime.h"
#include "chrome/browser/ui/browser_dialogs.h"
#include "chrome/common/chrome_switches.h"
#include "chrome/common/url_constants.h"
#include "components/url_fixer/url_fixer.h"

bool WillHandleBrowserAboutURL(GURL* url,
                               content::BrowserContext* browser_context) {
  // TODO(msw): Eliminate "about:*" constants and literals from code and tests,
  //            then hopefully we can remove this forced fixup.
  *url = url_fixer::FixupURL(url->possibly_invalid_spec(), std::string());

  // Check that about: URLs are fixed up to chrome: by url_fixer::FixupURL.
  DCHECK((*url == GURL(url::kAboutBlankURL)) ||
         !url->SchemeIs(url::kAboutScheme));

  // Only handle chrome://foo/, url_fixer::FixupURL translates about:foo.
  if (!url->SchemeIs(content::kChromeUIScheme))
    return false;

  std::string host(url->host());
  std::string path;
  // Replace about with chrome-urls.
  if (host == chrome::kChromeUIAboutHost)
    host = chrome::kChromeUIChromeURLsHost;
  // Replace cache with view-http-cache.
  if (host == chrome::kChromeUICacheHost) {
    host = content::kChromeUINetworkViewCacheHost;
  // Replace sync with sync-internals (for legacy reasons).
  } else if (host == chrome::kChromeUISyncHost) {
    host = chrome::kChromeUISyncInternalsHost;
  // Redirect chrome://extensions.
  } else if (host == chrome::kChromeUIExtensionsHost) {
    host = chrome::kChromeUIUberHost;
    path = chrome::kChromeUIExtensionsHost + url->path();
  // Redirect chrome://settings/extensions (legacy URL).
  } else if (host == chrome::kChromeUISettingsHost &&
      url->path() == std::string("/") + chrome::kExtensionsSubPage) {
    host = chrome::kChromeUIUberHost;
    path = chrome::kChromeUIExtensionsHost;
  // Redirect chrome://history.
  } else if (host == chrome::kChromeUIHistoryHost) {
#if defined(OS_ANDROID)
    // On Android, redirect directly to chrome://history-frame since
    // uber page is unsupported.
    host = chrome::kChromeUIHistoryFrameHost;
#else
    host = chrome::kChromeUIUberHost;
    path = chrome::kChromeUIHistoryHost + url->path();
#endif
  // Redirect chrome://settings
  } else if (host == chrome::kChromeUISettingsHost) {
    if (::switches::AboutInSettingsEnabled()) {
      host = chrome::kChromeUISettingsFrameHost;
    } else {
      host = chrome::kChromeUIUberHost;
      path = chrome::kChromeUISettingsHost + url->path();
    }
  // Redirect chrome://help
  } else if (host == chrome::kChromeUIHelpHost) {
    if (::switches::AboutInSettingsEnabled()) {
      host = chrome::kChromeUISettingsFrameHost;
      if (url->path().empty() || url->path() == "/")
        path = chrome::kChromeUIHelpHost;
    } else {
      host = chrome::kChromeUIUberHost;
      path = chrome::kChromeUIHelpHost + url->path();
    }
  }

  GURL::Replacements replacements;
  replacements.SetHostStr(host);
  if (!path.empty())
    replacements.SetPathStr(path);
  *url = url->ReplaceComponents(replacements);

  // Having re-written the URL, make the chrome: handler process it.
  return false;
}

bool HandleNonNavigationAboutURL(const GURL& url) {
  const std::string spec(url.spec());

  if (LowerCaseEqualsASCII(spec, chrome::kChromeUIRestartURL)) {
    // Call AttemptRestart after chrome::Navigate() completes to avoid access of
    // gtk objects after they are destroyed by BrowserWindowGtk::Close().
    base::MessageLoop::current()->PostTask(FROM_HERE,
        base::Bind(&chrome::AttemptRestart));
    return true;
  } else if (LowerCaseEqualsASCII(spec, chrome::kChromeUIQuitURL)) {
    base::MessageLoop::current()->PostTask(FROM_HERE,
        base::Bind(&chrome::AttemptExit));
    return true;
  }

  return false;
}
