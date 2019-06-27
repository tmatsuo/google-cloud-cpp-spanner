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

#include "google/cloud/spanner/sql_statement.h"
#include <algorithm>

namespace google {
namespace cloud {
namespace spanner {
inline namespace SPANNER_CLIENT_NS {

std::vector<std::string> SqlStatement::ParameterNames() const {
  std::vector<std::string> keys;
  keys.reserve(params_.size());
  for (auto const& p : params_) {
    keys.push_back(p.first);
  }
  return keys;
}

google::cloud::StatusOr<Value> SqlStatement::GetParameter(
    std::string const& parameter_name) const {
  auto iter = params_.find(parameter_name);
  if (iter != params_.end()) {
    return iter->second;
  }
  return Status(StatusCode::kNotFound, "No such parameter: " + parameter_name);
}

std::ostream& operator<<(std::ostream& os, SqlStatement const& stmt) {
  os << stmt.statement_;
  for (auto const& param : stmt.params_) {
    os << "\n[param]: {value}\t[" << param.first << "]: {";
    PrintTo(param.second, &os);
    os << "}";
  }
  return os;
}

}  // namespace SPANNER_CLIENT_NS
}  // namespace spanner
}  // namespace cloud
}  // namespace google