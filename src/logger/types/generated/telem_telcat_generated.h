// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TELEMTELCAT_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_TELEMTELCAT_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Telem_telcat_fb;
struct Telem_telcat_fbBuilder;

inline const ::flatbuffers::TypeTable *Telem_telcat_fbTypeTable();

struct Telem_telcat_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Telem_telcat_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Telem_telcat_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CATOBJ = 4,
    VT_CATRM = 6,
    VT_CATRA = 8,
    VT_CATDEC = 10,
    VT_CATEP = 12,
    VT_CATRO = 14
  };
  /// Catalog object name
  const ::flatbuffers::String *catObj() const {
    return GetPointer<const ::flatbuffers::String *>(VT_CATOBJ);
  }
  /// Catalog rotator mode
  const ::flatbuffers::String *catRm() const {
    return GetPointer<const ::flatbuffers::String *>(VT_CATRM);
  }
  /// Catalog right ascension [degrees]
  double catRa() const {
    return GetField<double>(VT_CATRA, 0.0);
  }
  /// Catalog declination [degrees]
  double catDec() const {
    return GetField<double>(VT_CATDEC, 0.0);
  }
  /// Catalog epoch
  double catEp() const {
    return GetField<double>(VT_CATEP, 0.0);
  }
  /// Catalog rotator offset
  double catRo() const {
    return GetField<double>(VT_CATRO, 0.0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_CATOBJ) &&
           verifier.VerifyString(catObj()) &&
           VerifyOffset(verifier, VT_CATRM) &&
           verifier.VerifyString(catRm()) &&
           VerifyField<double>(verifier, VT_CATRA, 8) &&
           VerifyField<double>(verifier, VT_CATDEC, 8) &&
           VerifyField<double>(verifier, VT_CATEP, 8) &&
           VerifyField<double>(verifier, VT_CATRO, 8) &&
           verifier.EndTable();
  }
};

struct Telem_telcat_fbBuilder {
  typedef Telem_telcat_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_catObj(::flatbuffers::Offset<::flatbuffers::String> catObj) {
    fbb_.AddOffset(Telem_telcat_fb::VT_CATOBJ, catObj);
  }
  void add_catRm(::flatbuffers::Offset<::flatbuffers::String> catRm) {
    fbb_.AddOffset(Telem_telcat_fb::VT_CATRM, catRm);
  }
  void add_catRa(double catRa) {
    fbb_.AddElement<double>(Telem_telcat_fb::VT_CATRA, catRa, 0.0);
  }
  void add_catDec(double catDec) {
    fbb_.AddElement<double>(Telem_telcat_fb::VT_CATDEC, catDec, 0.0);
  }
  void add_catEp(double catEp) {
    fbb_.AddElement<double>(Telem_telcat_fb::VT_CATEP, catEp, 0.0);
  }
  void add_catRo(double catRo) {
    fbb_.AddElement<double>(Telem_telcat_fb::VT_CATRO, catRo, 0.0);
  }
  explicit Telem_telcat_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Telem_telcat_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Telem_telcat_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Telem_telcat_fb> CreateTelem_telcat_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> catObj = 0,
    ::flatbuffers::Offset<::flatbuffers::String> catRm = 0,
    double catRa = 0.0,
    double catDec = 0.0,
    double catEp = 0.0,
    double catRo = 0.0) {
  Telem_telcat_fbBuilder builder_(_fbb);
  builder_.add_catRo(catRo);
  builder_.add_catEp(catEp);
  builder_.add_catDec(catDec);
  builder_.add_catRa(catRa);
  builder_.add_catRm(catRm);
  builder_.add_catObj(catObj);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Telem_telcat_fb> CreateTelem_telcat_fbDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *catObj = nullptr,
    const char *catRm = nullptr,
    double catRa = 0.0,
    double catDec = 0.0,
    double catEp = 0.0,
    double catRo = 0.0) {
  auto catObj__ = catObj ? _fbb.CreateString(catObj) : 0;
  auto catRm__ = catRm ? _fbb.CreateString(catRm) : 0;
  return MagAOX::logger::CreateTelem_telcat_fb(
      _fbb,
      catObj__,
      catRm__,
      catRa,
      catDec,
      catEp,
      catRo);
}

inline const ::flatbuffers::TypeTable *Telem_telcat_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_STRING, 0, -1 },
    { ::flatbuffers::ET_STRING, 0, -1 },
    { ::flatbuffers::ET_DOUBLE, 0, -1 },
    { ::flatbuffers::ET_DOUBLE, 0, -1 },
    { ::flatbuffers::ET_DOUBLE, 0, -1 },
    { ::flatbuffers::ET_DOUBLE, 0, -1 }
  };
  static const char * const names[] = {
    "catObj",
    "catRm",
    "catRa",
    "catDec",
    "catEp",
    "catRo"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 6, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Telem_telcat_fb *GetTelem_telcat_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Telem_telcat_fb>(buf);
}

inline const MagAOX::logger::Telem_telcat_fb *GetSizePrefixedTelem_telcat_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Telem_telcat_fb>(buf);
}

inline bool VerifyTelem_telcat_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Telem_telcat_fb>(nullptr);
}

inline bool VerifySizePrefixedTelem_telcat_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Telem_telcat_fb>(nullptr);
}

inline void FinishTelem_telcat_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_telcat_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedTelem_telcat_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Telem_telcat_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_TELEMTELCAT_MAGAOX_LOGGER_H_