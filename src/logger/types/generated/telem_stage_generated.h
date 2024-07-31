// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TELEMSTAGE_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_TELEMSTAGE_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Telem_stage_fb;
struct Telem_stage_fbBuilder;

inline const ::flatbuffers::TypeTable *Telem_stage_fbTypeTable();

struct Telem_stage_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Telem_stage_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Telem_stage_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_MOVING = 4,
    VT_PRESET = 6,
    VT_PRESETNAME = 8
  };
  /// whether or not stage is in motion
  int8_t moving() const {
    return GetField<int8_t>(VT_MOVING, 0);
  }
  /// current position of stage in preset units
  float preset() const {
    return GetField<float>(VT_PRESET, 0.0f);
  }
  /// current preset name
  const ::flatbuffers::String *presetName() const {
    return GetPointer<const ::flatbuffers::String *>(VT_PRESETNAME);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int8_t>(verifier, VT_MOVING, 1) &&
           VerifyField<float>(verifier, VT_PRESET, 4) &&
           VerifyOffset(verifier, VT_PRESETNAME) &&
           verifier.VerifyString(presetName()) &&
           verifier.EndTable();
  }
};

struct Telem_stage_fbBuilder {
  typedef Telem_stage_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_moving(int8_t moving) {
    fbb_.AddElement<int8_t>(Telem_stage_fb::VT_MOVING, moving, 0);
  }
  void add_preset(float preset) {
    fbb_.AddElement<float>(Telem_stage_fb::VT_PRESET, preset, 0.0f);
  }
  void add_presetName(::flatbuffers::Offset<::flatbuffers::String> presetName) {
    fbb_.AddOffset(Telem_stage_fb::VT_PRESETNAME, presetName);
  }
  explicit Telem_stage_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Telem_stage_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Telem_stage_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Telem_stage_fb> CreateTelem_stage_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int8_t moving = 0,
    float preset = 0.0f,
    ::flatbuffers::Offset<::flatbuffers::String> presetName = 0) {
  Telem_stage_fbBuilder builder_(_fbb);
  builder_.add_presetName(presetName);
  builder_.add_preset(preset);
  builder_.add_moving(moving);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Telem_stage_fb> CreateTelem_stage_fbDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    int8_t moving = 0,
    float preset = 0.0f,
    const char *presetName = nullptr) {
  auto presetName__ = presetName ? _fbb.CreateString(presetName) : 0;
  return MagAOX::logger::CreateTelem_stage_fb(
      _fbb,
      moving,
      preset,
      presetName__);
}

inline const ::flatbuffers::TypeTable *Telem_stage_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_CHAR, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_STRING, 0, -1 }
  };
  static const char * const names[] = {
    "moving",
    "preset",
    "presetName"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 3, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Telem_stage_fb *GetTelem_stage_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Telem_stage_fb>(buf);
}

inline const MagAOX::logger::Telem_stage_fb *GetSizePrefixedTelem_stage_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Telem_stage_fb>(buf);
}

inline bool VerifyTelem_stage_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Telem_stage_fb>(nullptr);
}

inline bool VerifySizePrefixedTelem_stage_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Telem_stage_fb>(nullptr);
}

inline void FinishTelem_stage_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_stage_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTelem_stage_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_stage_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_TELEMSTAGE_MAGAOX_LOGGER_H_