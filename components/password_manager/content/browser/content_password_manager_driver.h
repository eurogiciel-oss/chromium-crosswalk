// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PASSWORD_MANAGER_CONTENT_BROWSER_CONTENT_PASSWORD_MANAGER_DRIVER_H_
#define COMPONENTS_PASSWORD_MANAGER_CONTENT_BROWSER_CONTENT_PASSWORD_MANAGER_DRIVER_H_

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "components/password_manager/core/browser/password_autofill_manager.h"
#include "components/password_manager/core/browser/password_generation_manager.h"
#include "components/password_manager/core/browser/password_manager.h"
#include "components/password_manager/core/browser/password_manager_driver.h"
#include "content/public/browser/web_contents_observer.h"

namespace autofill {
class AutofillManager;
struct PasswordForm;
}

namespace content {
class WebContents;
}

namespace password_manager {

class ContentPasswordManagerDriver : public PasswordManagerDriver,
                                     public content::WebContentsObserver {
 public:
  ContentPasswordManagerDriver(content::WebContents* web_contents,
                               PasswordManagerClient* client,
                               autofill::AutofillClient* autofill_client);
  virtual ~ContentPasswordManagerDriver();

  // PasswordManagerDriver implementation.
  virtual void FillPasswordForm(const autofill::PasswordFormFillData& form_data)
      override;
  virtual bool DidLastPageLoadEncounterSSLErrors() override;
  virtual bool IsOffTheRecord() override;
  virtual void AllowPasswordGenerationForForm(
      const autofill::PasswordForm& form) override;
  virtual void AccountCreationFormsFound(
      const std::vector<autofill::FormData>& forms) override;
  virtual void FillSuggestion(const base::string16& username,
                              const base::string16& password) override;
  virtual void PreviewSuggestion(const base::string16& username,
                                 const base::string16& password) override;
  virtual void ClearPreviewedForm() override;

  virtual PasswordGenerationManager* GetPasswordGenerationManager() override;
  virtual PasswordManager* GetPasswordManager() override;
  virtual autofill::AutofillManager* GetAutofillManager() override;
  virtual PasswordAutofillManager* GetPasswordAutofillManager() override;

  // content::WebContentsObserver overrides.
  virtual bool OnMessageReceived(const IPC::Message& message) override;
  virtual void DidNavigateMainFrame(
      const content::LoadCommittedDetails& details,
      const content::FrameNavigateParams& params) override;

 private:
  PasswordManager password_manager_;
  PasswordGenerationManager password_generation_manager_;
  PasswordAutofillManager password_autofill_manager_;

  DISALLOW_COPY_AND_ASSIGN(ContentPasswordManagerDriver);
};

}  // namespace password_manager

#endif  // COMPONENTS_PASSWORD_MANAGER_CONTENT_BROWSER_CONTENT_PASSWORD_MANAGER_DRIVER_H_
