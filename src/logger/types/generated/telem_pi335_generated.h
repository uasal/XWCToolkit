// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TELEMPI335_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_TELEMPI335_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Telem_pi335_fb;
struct Telem_pi335_fbBuilder;

inline const ::flatbuffers::TypeTable *Telem_pi335_fbTypeTable();

struct Telem_pi335_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Telem_pi335_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Telem_pi335_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_POS1SET = 4,
    VT_POS1 = 6,
    VT_SVA1 = 8,
    VT_POS2SET = 10,
    VT_POS2 = 12,
    VT_SVA2 = 14,
    VT_POS3SET = 16,
    VT_POS3 = 18,
    VT_SVA3 = 20
  };
  /// position
  float pos1Set() const {
    return GetField<float>(VT_POS1SET, 0.0f);
  }
  float pos1() const {
    return GetField<float>(VT_POS1, 0.0f);
  }
  float sva1() const {
    return GetField<float>(VT_SVA1, 0.0f);
  }
  float pos2Set() const {
    return GetField<float>(VT_POS2SET, 0.0f);
  }
  float pos2() const {
    return GetField<float>(VT_POS2, 0.0f);
  }
  float sva2() const {
    return GetField<float>(VT_SVA2, 0.0f);
  }
  float pos3Set() const {
    return GetField<float>(VT_POS3SET, 0.0f);
  }
  float pos3() const {
    return GetField<float>(VT_POS3, 0.0f);
  }
  float sva3() const {
    return GetField<float>(VT_SVA3, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_POS1SET, 4) &&
           VerifyField<float>(verifier, VT_POS1, 4) &&
           VerifyField<float>(verifier, VT_SVA1, 4) &&
           VerifyField<float>(verifier, VT_POS2SET, 4) &&
           VerifyField<float>(verifier, VT_POS2, 4) &&
           VerifyField<float>(verifier, VT_SVA2, 4) &&
           VerifyField<float>(verifier, VT_POS3SET, 4) &&
           VerifyField<float>(verifier, VT_POS3, 4) &&
           VerifyField<float>(verifier, VT_SVA3, 4) &&
           verifier.EndTable();
  }
};

struct Telem_pi335_fbBuilder {
  typedef Telem_pi335_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_pos1Set(float pos1Set) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS1SET, pos1Set, 0.0f);
  }
  void add_pos1(float pos1) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS1, pos1, 0.0f);
  }
  void add_sva1(float sva1) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_SVA1, sva1, 0.0f);
  }
  void add_pos2Set(float pos2Set) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS2SET, pos2Set, 0.0f);
  }
  void add_pos2(float pos2) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS2, pos2, 0.0f);
  }
  void add_sva2(float sva2) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_SVA2, sva2, 0.0f);
  }
  void add_pos3Set(float pos3Set) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS3SET, pos3Set, 0.0f);
  }
  void add_pos3(float pos3) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_POS3, pos3, 0.0f);
  }
  void add_sva3(float sva3) {
    fbb_.AddElement<float>(Telem_pi335_fb::VT_SVA3, sva3, 0.0f);
  }
  explicit Telem_pi335_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Telem_pi335_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Telem_pi335_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Telem_pi335_fb> CreateTelem_pi335_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float pos1Set = 0.0f,
    float pos1 = 0.0f,
    float sva1 = 0.0f,
    float pos2Set = 0.0f,
    float pos2 = 0.0f,
    float sva2 = 0.0f,
    float pos3Set = 0.0f,
    float pos3 = 0.0f,
    float sva3 = 0.0f) {
  Telem_pi335_fbBuilder builder_(_fbb);
  builder_.add_sva3(sva3);
  builder_.add_pos3(pos3);
  builder_.add_pos3Set(pos3Set);
  builder_.add_sva2(sva2);
  builder_.add_pos2(pos2);
  builder_.add_pos2Set(pos2Set);
  builder_.add_sva1(sva1);
  builder_.add_pos1(pos1);
  builder_.add_pos1Set(pos1Set);
  return builder_.Finish();
}

inline const ::flatbuffers::TypeTable *Telem_pi335_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 }
  };
  static const char * const names[] = {
    "pos1Set",
    "pos1",
    "sva1",
    "pos2Set",
    "pos2",
    "sva2",
    "pos3Set",
    "pos3",
    "sva3"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 9, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Telem_pi335_fb *GetTelem_pi335_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Telem_pi335_fb>(buf);
}

inline const MagAOX::logger::Telem_pi335_fb *GetSizePrefixedTelem_pi335_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Telem_pi335_fb>(buf);
}

inline bool VerifyTelem_pi335_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Telem_pi335_fb>(nullptr);
}

inline bool VerifySizePrefixedTelem_pi335_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Telem_pi335_fb>(nullptr);
}

inline void FinishTelem_pi335_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_pi335_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTelem_pi335_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_pi335_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_TELEMPI335_MAGAOX_LOGGER_H_