// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TELEMRHUSB_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_TELEMRHUSB_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Telem_rhusb_fb;
struct Telem_rhusb_fbBuilder;

inline const ::flatbuffers::TypeTable *Telem_rhusb_fbTypeTable();

struct Telem_rhusb_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Telem_rhusb_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Telem_rhusb_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEMP = 4,
    VT_RH = 6
  };
  float temp() const {
    return GetField<float>(VT_TEMP, 0.0f);
  }
  float rh() const {
    return GetField<float>(VT_RH, 0.0f);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_TEMP, 4) &&
           VerifyField<float>(verifier, VT_RH, 4) &&
           verifier.EndTable();
  }
};

struct Telem_rhusb_fbBuilder {
  typedef Telem_rhusb_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_temp(float temp) {
    fbb_.AddElement<float>(Telem_rhusb_fb::VT_TEMP, temp, 0.0f);
  }
  void add_rh(float rh) {
    fbb_.AddElement<float>(Telem_rhusb_fb::VT_RH, rh, 0.0f);
  }
  explicit Telem_rhusb_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Telem_rhusb_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Telem_rhusb_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Telem_rhusb_fb> CreateTelem_rhusb_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    float temp = 0.0f,
    float rh = 0.0f) {
  Telem_rhusb_fbBuilder builder_(_fbb);
  builder_.add_rh(rh);
  builder_.add_temp(temp);
  return builder_.Finish();
}

inline const ::flatbuffers::TypeTable *Telem_rhusb_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_FLOAT, 0, -1 },
    { ::flatbuffers::ET_FLOAT, 0, -1 }
  };
  static const char * const names[] = {
    "temp",
    "rh"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 2, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Telem_rhusb_fb *GetTelem_rhusb_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Telem_rhusb_fb>(buf);
}

inline const MagAOX::logger::Telem_rhusb_fb *GetSizePrefixedTelem_rhusb_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Telem_rhusb_fb>(buf);
}

inline bool VerifyTelem_rhusb_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Telem_rhusb_fb>(nullptr);
}

inline bool VerifySizePrefixedTelem_rhusb_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Telem_rhusb_fb>(nullptr);
}

inline void FinishTelem_rhusb_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_rhusb_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTelem_rhusb_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_rhusb_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_TELEMRHUSB_MAGAOX_LOGGER_H_