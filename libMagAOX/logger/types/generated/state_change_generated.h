// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_STATECHANGE_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_STATECHANGE_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct State_change_fb;
struct State_change_fbBuilder;

inline const ::flatbuffers::TypeTable *State_change_fbTypeTable();

struct State_change_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef State_change_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return State_change_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_FROM = 4,
    VT_TO = 6
  };
  int16_t from() const {
    return GetField<int16_t>(VT_FROM, 0);
  }
  int16_t to() const {
    return GetField<int16_t>(VT_TO, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int16_t>(verifier, VT_FROM, 2) &&
           VerifyField<int16_t>(verifier, VT_TO, 2) &&
           verifier.EndTable();
  }
};

struct State_change_fbBuilder {
  typedef State_change_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_from(int16_t from) {
    fbb_.AddElement<int16_t>(State_change_fb::VT_FROM, from, 0);
  }
  void add_to(int16_t to) {
    fbb_.AddElement<int16_t>(State_change_fb::VT_TO, to, 0);
  }
  explicit State_change_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<State_change_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<State_change_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<State_change_fb> CreateState_change_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int16_t from = 0,
    int16_t to = 0) {
  State_change_fbBuilder builder_(_fbb);
  builder_.add_to(to);
  builder_.add_from(from);
  return builder_.Finish();
}

inline const ::flatbuffers::TypeTable *State_change_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_SHORT, 0, -1 },
    { ::flatbuffers::ET_SHORT, 0, -1 }
  };
  static const char * const names[] = {
    "from",
    "to"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::State_change_fb *GetState_change_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::State_change_fb>(buf);
}

inline const MagAOX::logger::State_change_fb *GetSizePrefixedState_change_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::State_change_fb>(buf);
}

inline bool VerifyState_change_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::State_change_fb>(nullptr);
}

inline bool VerifySizePrefixedState_change_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::State_change_fb>(nullptr);
}

inline void FinishState_change_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::State_change_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedState_change_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::State_change_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_STATECHANGE_MAGAOX_LOGGER_H_