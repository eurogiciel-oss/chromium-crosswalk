// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_PASSWORD_STORE_DEFAULT_H_
#define COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_PASSWORD_STORE_DEFAULT_H_

#include <vector>

#include "base/memory/scoped_ptr.h"
#include "components/password_manager/core/browser/login_database.h"
#include "components/password_manager/core/browser/password_store.h"

namespace password_manager {

// Simple password store implementation that delegates everything to
// the LoginDatabase.
class PasswordStoreDefault : public PasswordStore {
 public:
  // Takes ownership of |login_db|.
  PasswordStoreDefault(
      scoped_refptr<base::SingleThreadTaskRunner> main_thread_runner,
      scoped_refptr<base::SingleThreadTaskRunner> db_thread_runner,
      LoginDatabase* login_db);

 protected:
  virtual ~PasswordStoreDefault();

  // Implements PasswordStore interface.
  virtual void ReportMetricsImpl(const std::string& sync_username) override;
  virtual PasswordStoreChangeList AddLoginImpl(
      const autofill::PasswordForm& form) override;
  virtual PasswordStoreChangeList UpdateLoginImpl(
      const autofill::PasswordForm& form) override;
  virtual PasswordStoreChangeList RemoveLoginImpl(
      const autofill::PasswordForm& form) override;
  virtual PasswordStoreChangeList RemoveLoginsCreatedBetweenImpl(
      base::Time delete_begin,
      base::Time delete_end) override;
  virtual PasswordStoreChangeList RemoveLoginsSyncedBetweenImpl(
      base::Time delete_begin,
      base::Time delete_end) override;
  virtual void GetLoginsImpl(
      const autofill::PasswordForm& form,
      AuthorizationPromptPolicy prompt_policy,
      const ConsumerCallbackRunner& callback_runner) override;
  virtual void GetAutofillableLoginsImpl(GetLoginsRequest* request) override;
  virtual void GetBlacklistLoginsImpl(GetLoginsRequest* request) override;
  virtual bool FillAutofillableLogins(
      std::vector<autofill::PasswordForm*>* forms) override;
  virtual bool FillBlacklistLogins(
      std::vector<autofill::PasswordForm*>* forms) override;

 protected:
  inline bool DeleteAndRecreateDatabaseFile() {
    return login_db_->DeleteAndRecreateDatabaseFile();
  }

 private:
  scoped_ptr<LoginDatabase> login_db_;

  DISALLOW_COPY_AND_ASSIGN(PasswordStoreDefault);
};

}  // namespace password_manager

#endif  // COMPONENTS_PASSWORD_MANAGER_CORE_BROWSER_PASSWORD_STORE_DEFAULT_H_
