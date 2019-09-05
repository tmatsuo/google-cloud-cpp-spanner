// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_CLOUD_CPP_SPANNER_GOOGLE_CLOUD_SPANNER_TESTING_MOCK_INSTANCE_ADMIN_STUB_H_
#define GOOGLE_CLOUD_CPP_SPANNER_GOOGLE_CLOUD_SPANNER_TESTING_MOCK_INSTANCE_ADMIN_STUB_H_

#include "google/cloud/spanner/internal/instance_admin_stub.h"
#include <gmock/gmock.h>

namespace google {
namespace cloud {
namespace spanner_testing {
inline namespace SPANNER_CLIENT_NS {
class MockInstanceAdminStub
    : public google::cloud::spanner::internal::InstanceAdminStub {
 public:
  MOCK_METHOD2(
      GetInstance,
      StatusOr<google::spanner::admin::instance::v1::Instance>(
          grpc::ClientContext&,
          google::spanner::admin::instance::v1::GetInstanceRequest const&));
};

}  // namespace SPANNER_CLIENT_NS
}  // namespace spanner_testing
}  // namespace cloud
}  // namespace google

#endif  // GOOGLE_CLOUD_CPP_SPANNER_GOOGLE_CLOUD_SPANNER_TESTING_MOCK_INSTANCE_ADMIN_STUB_H_