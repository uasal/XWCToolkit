// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TELEMTEMPS_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_TELEMTEMPS_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Telem_temps_fb;
struct Telem_temps_fbBuilder;

inline const ::flatbuffers::TypeTable *Telem_temps_fbTypeTable();

struct Telem_temps_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Telem_temps_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Telem_temps_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TEMPS = 4
  };
  const ::flatbuffers::Vector<float> *temps() const {
    return GetPointer<const ::flatbuffers::Vector<float> *>(VT_TEMPS);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_TEMPS) &&
           verifier.VerifyVector(temps()) &&
           verifier.EndTable();
  }
};

struct Telem_temps_fbBuilder {
  typedef Telem_temps_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_temps(::flatbuffers::Offset<::flatbuffers::Vector<float>> temps) {
    fbb_.AddOffset(Telem_temps_fb::VT_TEMPS, temps);
  }
  explicit Telem_temps_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Telem_temps_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Telem_temps_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Telem_temps_fb> CreateTelem_temps_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::Vector<float>> temps = 0) {
  Telem_temps_fbBuilder builder_(_fbb);
  builder_.add_temps(temps);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Telem_temps_fb> CreateTelem_temps_fbDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<float> *temps = nullptr) {
  auto temps__ = temps ? _fbb.CreateVector<float>(*temps) : 0;
  return MagAOX::logger::CreateTelem_temps_fb(
      _fbb,
      temps__);
}

inline const ::flatbuffers::TypeTable *Telem_temps_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_FLOAT, 1, -1 }
  };
  static const char * const names[] = {
    "temps"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 1, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Telem_temps_fb *GetTelem_temps_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Telem_temps_fb>(buf);
}

inline const MagAOX::logger::Telem_temps_fb *GetSizePrefixedTelem_temps_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Telem_temps_fb>(buf);
}

inline bool VerifyTelem_temps_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Telem_temps_fb>(nullptr);
}

inline bool VerifySizePrefixedTelem_temps_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Telem_temps_fb>(nullptr);
}

inline void FinishTelem_temps_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_temps_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTelem_temps_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_temps_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_TELEMTEMPS_MAGAOX_LOGGER_H_