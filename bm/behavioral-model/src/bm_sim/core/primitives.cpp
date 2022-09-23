/* Copyright 2013-present Barefoot Networks, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Antonin Bas (antonin@barefootnetworks.com)
 *
 */

#include <bm/bm_sim/core/primitives.h>

namespace bm {

namespace core {

REGISTER_PRIMITIVE(assign);

REGISTER_PRIMITIVE(assign_VL);

REGISTER_PRIMITIVE(assign_header);

void
assign_header::operator ()(Header &dst, const Header &src) {
  if (!src.is_valid()) {
    dst.mark_invalid();
    return;
  }
  dst.mark_valid();
  assert(dst.get_header_type_id() == src.get_header_type_id());
  for (unsigned int i = 0; i < dst.size(); i++) {
    if (dst[i].is_VL())
      dst[i].assign_VL(src[i]);
    else
      dst[i].set(src[i]);
  }
}

// dummy method to prevent the removal of global variables (generated by
// REGISTER_PRIMITIVE) by the linker; we call it in the ActionOpcodesMap
// constructor in actions.cpp
int
_bm_core_primitives_import() { return 0; }

}  // namespace core

}  // namespace bm
