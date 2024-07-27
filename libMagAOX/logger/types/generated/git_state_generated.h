// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_GITSTATE_MAGAOX_LOGGER_H_
#define FLATBUFFERS_GENERATED_GITSTATE_MAGAOX_LOGGER_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 23 &&
              FLATBUFFERS_VERSION_MINOR == 5 &&
              FLATBUFFERS_VERSION_REVISION == 26,
             "Non-compatible flatbuffers version included");

namespace MagAOX {
namespace logger {

struct Git_state_fb;
struct Git_state_fbBuilder;

inline const ::flatbuffers::TypeTable *Git_state_fbTypeTable();

struct Git_state_fb FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef Git_state_fbBuilder Builder;
  static const ::flatbuffers::TypeTable *MiniReflectTypeTable() {
    return Git_state_fbTypeTable();
  }
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_REPO = 4,
    VT_SHA1 = 6,
    VT_MODIFIED = 8
  };
  const ::flatbuffers::String *repo() const {
    return GetPointer<const ::flatbuffers::String *>(VT_REPO);
  }
  const ::flatbuffers::String *sha1() const {
    return GetPointer<const ::flatbuffers::String *>(VT_SHA1);
  }
  uint8_t modified() const {
    return GetField<uint8_t>(VT_MODIFIED, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_REPO) &&
           verifier.VerifyString(repo()) &&
           VerifyOffset(verifier, VT_SHA1) &&
           verifier.VerifyString(sha1()) &&
           VerifyField<uint8_t>(verifier, VT_MODIFIED, 1) &&
           verifier.EndTable();
  }
};

struct Git_state_fbBuilder {
  typedef Git_state_fb Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_repo(::flatbuffers::Offset<::flatbuffers::String> repo) {
    fbb_.AddOffset(Git_state_fb::VT_REPO, repo);
  }
  void add_sha1(::flatbuffers::Offset<::flatbuffers::String> sha1) {
    fbb_.AddOffset(Git_state_fb::VT_SHA1, sha1);
  }
  void add_modified(uint8_t modified) {
    fbb_.AddElement<uint8_t>(Git_state_fb::VT_MODIFIED, modified, 0);
  }
  explicit Git_state_fbBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Git_state_fb> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Git_state_fb>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Git_state_fb> CreateGit_state_fb(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    ::flatbuffers::Offset<::flatbuffers::String> repo = 0,
    ::flatbuffers::Offset<::flatbuffers::String> sha1 = 0,
    uint8_t modified = 0) {
  Git_state_fbBuilder builder_(_fbb);
  builder_.add_sha1(sha1);
  builder_.add_repo(repo);
  builder_.add_modified(modified);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Git_state_fb> CreateGit_state_fbDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    const char *repo = nullptr,
    const char *sha1 = nullptr,
    uint8_t modified = 0) {
  auto repo__ = repo ? _fbb.CreateString(repo) : 0;
  auto sha1__ = sha1 ? _fbb.CreateString(sha1) : 0;
  return MagAOX::logger::CreateGit_state_fb(
      _fbb,
      repo__,
      sha1__,
      modified);
}

inline const ::flatbuffers::TypeTable *Git_state_fbTypeTable() {
  static const ::flatbuffers::TypeCode type_codes[] = {
    { ::flatbuffers::ET_STRING, 0, -1 },
    { ::flatbuffers::ET_STRING, 0, -1 },
    { ::flatbuffers::ET_UCHAR, 0, -1 }
  };
  static const char * const names[] = {
    "repo",
    "sha1",
    "modified"
  };
  static const ::flatbuffers::TypeTable tt = {
    ::flatbuffers::ST_TABLE, 3, type_codes, nullptr, nullptr, nullptr, names
  };
  return &tt;
}

inline const MagAOX::logger::Git_state_fb *GetGit_state_fb(const void *buf) {
  return ::flatbuffers::GetRoot<MagAOX::logger::Git_state_fb>(buf);
}

inline const MagAOX::logger::Git_state_fb *GetSizePrefixedGit_state_fb(const void *buf) {
  return ::flatbuffers::GetSizePrefixedRoot<MagAOX::logger::Git_state_fb>(buf);
}

inline bool VerifyGit_state_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<MagAOX::logger::Git_state_fb>(nullptr);
}

inline bool VerifySizePrefixedGit_state_fbBuffer(
    ::flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<MagAOX::logger::Git_state_fb>(nullptr);
}

inline void FinishGit_state_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Git_state_fb> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedGit_state_fbBuffer(
    ::flatbuffers::FlatBufferBuilder &fbb,
    ::flatbuffers::Offset<MagAOX::logger::Git_state_fb> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace logger
}  // namespace MagAOX

#endif  // FLATBUFFERS_GENERATED_GITSTATE_MAGAOX_LOGGER_H_