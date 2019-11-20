// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AppSendInfo.proto

#ifndef PROTOBUF_AppSendInfo_2eproto__INCLUDED
#define PROTOBUF_AppSendInfo_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace com {
namespace eightplus {
namespace appinfo {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_AppSendInfo_2eproto();
void protobuf_AssignDesc_AppSendInfo_2eproto();
void protobuf_ShutdownFile_AppSendInfo_2eproto();

class Notification;
class RunningAppItem;
class RunningAppList;

// ===================================================================

class Notification : public ::google::protobuf::Message {
 public:
  Notification();
  virtual ~Notification();

  Notification(const Notification& from);

  inline Notification& operator=(const Notification& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Notification& default_instance();

  void Swap(Notification* other);

  // implements Message ----------------------------------------------

  Notification* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Notification& from);
  void MergeFrom(const Notification& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string package_name = 1;
  inline bool has_package_name() const;
  inline void clear_package_name();
  static const int kPackageNameFieldNumber = 1;
  inline const ::std::string& package_name() const;
  inline void set_package_name(const ::std::string& value);
  inline void set_package_name(const char* value);
  inline void set_package_name(const char* value, size_t size);
  inline ::std::string* mutable_package_name();
  inline ::std::string* release_package_name();
  inline void set_allocated_package_name(::std::string* package_name);

  // required string app_name = 2;
  inline bool has_app_name() const;
  inline void clear_app_name();
  static const int kAppNameFieldNumber = 2;
  inline const ::std::string& app_name() const;
  inline void set_app_name(const ::std::string& value);
  inline void set_app_name(const char* value);
  inline void set_app_name(const char* value, size_t size);
  inline ::std::string* mutable_app_name();
  inline ::std::string* release_app_name();
  inline void set_allocated_app_name(::std::string* app_name);

  // required string text = 3;
  inline bool has_text() const;
  inline void clear_text();
  static const int kTextFieldNumber = 3;
  inline const ::std::string& text() const;
  inline void set_text(const ::std::string& value);
  inline void set_text(const char* value);
  inline void set_text(const char* value, size_t size);
  inline ::std::string* mutable_text();
  inline ::std::string* release_text();
  inline void set_allocated_text(::std::string* text);

  // @@protoc_insertion_point(class_scope:com.eightplus.appinfo.Notification)
 private:
  inline void set_has_package_name();
  inline void clear_has_package_name();
  inline void set_has_app_name();
  inline void clear_has_app_name();
  inline void set_has_text();
  inline void clear_has_text();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* package_name_;
  ::std::string* app_name_;
  ::std::string* text_;
  friend void  protobuf_AddDesc_AppSendInfo_2eproto();
  friend void protobuf_AssignDesc_AppSendInfo_2eproto();
  friend void protobuf_ShutdownFile_AppSendInfo_2eproto();

  void InitAsDefaultInstance();
  static Notification* default_instance_;
};
// -------------------------------------------------------------------

class RunningAppItem : public ::google::protobuf::Message {
 public:
  RunningAppItem();
  virtual ~RunningAppItem();

  RunningAppItem(const RunningAppItem& from);

  inline RunningAppItem& operator=(const RunningAppItem& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RunningAppItem& default_instance();

  void Swap(RunningAppItem* other);

  // implements Message ----------------------------------------------

  RunningAppItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RunningAppItem& from);
  void MergeFrom(const RunningAppItem& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string app_name = 1;
  inline bool has_app_name() const;
  inline void clear_app_name();
  static const int kAppNameFieldNumber = 1;
  inline const ::std::string& app_name() const;
  inline void set_app_name(const ::std::string& value);
  inline void set_app_name(const char* value);
  inline void set_app_name(const char* value, size_t size);
  inline ::std::string* mutable_app_name();
  inline ::std::string* release_app_name();
  inline void set_allocated_app_name(::std::string* app_name);

  // required string package_name = 2;
  inline bool has_package_name() const;
  inline void clear_package_name();
  static const int kPackageNameFieldNumber = 2;
  inline const ::std::string& package_name() const;
  inline void set_package_name(const ::std::string& value);
  inline void set_package_name(const char* value);
  inline void set_package_name(const char* value, size_t size);
  inline ::std::string* mutable_package_name();
  inline ::std::string* release_package_name();
  inline void set_allocated_package_name(::std::string* package_name);

  // @@protoc_insertion_point(class_scope:com.eightplus.appinfo.RunningAppItem)
 private:
  inline void set_has_app_name();
  inline void clear_has_app_name();
  inline void set_has_package_name();
  inline void clear_has_package_name();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::std::string* app_name_;
  ::std::string* package_name_;
  friend void  protobuf_AddDesc_AppSendInfo_2eproto();
  friend void protobuf_AssignDesc_AppSendInfo_2eproto();
  friend void protobuf_ShutdownFile_AppSendInfo_2eproto();

  void InitAsDefaultInstance();
  static RunningAppItem* default_instance_;
};
// -------------------------------------------------------------------

class RunningAppList : public ::google::protobuf::Message {
 public:
  RunningAppList();
  virtual ~RunningAppList();

  RunningAppList(const RunningAppList& from);

  inline RunningAppList& operator=(const RunningAppList& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const RunningAppList& default_instance();

  void Swap(RunningAppList* other);

  // implements Message ----------------------------------------------

  RunningAppList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RunningAppList& from);
  void MergeFrom(const RunningAppList& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .com.eightplus.appinfo.RunningAppItem item = 1;
  inline int item_size() const;
  inline void clear_item();
  static const int kItemFieldNumber = 1;
  inline const ::com::eightplus::appinfo::RunningAppItem& item(int index) const;
  inline ::com::eightplus::appinfo::RunningAppItem* mutable_item(int index);
  inline ::com::eightplus::appinfo::RunningAppItem* add_item();
  inline const ::google::protobuf::RepeatedPtrField< ::com::eightplus::appinfo::RunningAppItem >&
      item() const;
  inline ::google::protobuf::RepeatedPtrField< ::com::eightplus::appinfo::RunningAppItem >*
      mutable_item();

  // @@protoc_insertion_point(class_scope:com.eightplus.appinfo.RunningAppList)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::com::eightplus::appinfo::RunningAppItem > item_;
  friend void  protobuf_AddDesc_AppSendInfo_2eproto();
  friend void protobuf_AssignDesc_AppSendInfo_2eproto();
  friend void protobuf_ShutdownFile_AppSendInfo_2eproto();

  void InitAsDefaultInstance();
  static RunningAppList* default_instance_;
};
// ===================================================================


// ===================================================================

// Notification

// required string package_name = 1;
inline bool Notification::has_package_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Notification::set_has_package_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Notification::clear_has_package_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Notification::clear_package_name() {
  if (package_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_->clear();
  }
  clear_has_package_name();
}
inline const ::std::string& Notification::package_name() const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.Notification.package_name)
  return *package_name_;
}
inline void Notification::set_package_name(const ::std::string& value) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(value);
  // @@protoc_insertion_point(field_set:com.eightplus.appinfo.Notification.package_name)
}
inline void Notification::set_package_name(const char* value) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.eightplus.appinfo.Notification.package_name)
}
inline void Notification::set_package_name(const char* value, size_t size) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.eightplus.appinfo.Notification.package_name)
}
inline ::std::string* Notification::mutable_package_name() {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.Notification.package_name)
  return package_name_;
}
inline ::std::string* Notification::release_package_name() {
  clear_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = package_name_;
    package_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Notification::set_allocated_package_name(::std::string* package_name) {
  if (package_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete package_name_;
  }
  if (package_name) {
    set_has_package_name();
    package_name_ = package_name;
  } else {
    clear_has_package_name();
    package_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.eightplus.appinfo.Notification.package_name)
}

// required string app_name = 2;
inline bool Notification::has_app_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Notification::set_has_app_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Notification::clear_has_app_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Notification::clear_app_name() {
  if (app_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_->clear();
  }
  clear_has_app_name();
}
inline const ::std::string& Notification::app_name() const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.Notification.app_name)
  return *app_name_;
}
inline void Notification::set_app_name(const ::std::string& value) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(value);
  // @@protoc_insertion_point(field_set:com.eightplus.appinfo.Notification.app_name)
}
inline void Notification::set_app_name(const char* value) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.eightplus.appinfo.Notification.app_name)
}
inline void Notification::set_app_name(const char* value, size_t size) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.eightplus.appinfo.Notification.app_name)
}
inline ::std::string* Notification::mutable_app_name() {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.Notification.app_name)
  return app_name_;
}
inline ::std::string* Notification::release_app_name() {
  clear_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = app_name_;
    app_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Notification::set_allocated_app_name(::std::string* app_name) {
  if (app_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete app_name_;
  }
  if (app_name) {
    set_has_app_name();
    app_name_ = app_name;
  } else {
    clear_has_app_name();
    app_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.eightplus.appinfo.Notification.app_name)
}

// required string text = 3;
inline bool Notification::has_text() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Notification::set_has_text() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Notification::clear_has_text() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Notification::clear_text() {
  if (text_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    text_->clear();
  }
  clear_has_text();
}
inline const ::std::string& Notification::text() const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.Notification.text)
  return *text_;
}
inline void Notification::set_text(const ::std::string& value) {
  set_has_text();
  if (text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    text_ = new ::std::string;
  }
  text_->assign(value);
  // @@protoc_insertion_point(field_set:com.eightplus.appinfo.Notification.text)
}
inline void Notification::set_text(const char* value) {
  set_has_text();
  if (text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    text_ = new ::std::string;
  }
  text_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.eightplus.appinfo.Notification.text)
}
inline void Notification::set_text(const char* value, size_t size) {
  set_has_text();
  if (text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    text_ = new ::std::string;
  }
  text_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.eightplus.appinfo.Notification.text)
}
inline ::std::string* Notification::mutable_text() {
  set_has_text();
  if (text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    text_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.Notification.text)
  return text_;
}
inline ::std::string* Notification::release_text() {
  clear_has_text();
  if (text_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = text_;
    text_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void Notification::set_allocated_text(::std::string* text) {
  if (text_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete text_;
  }
  if (text) {
    set_has_text();
    text_ = text;
  } else {
    clear_has_text();
    text_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.eightplus.appinfo.Notification.text)
}

// -------------------------------------------------------------------

// RunningAppItem

// required string app_name = 1;
inline bool RunningAppItem::has_app_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RunningAppItem::set_has_app_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RunningAppItem::clear_has_app_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RunningAppItem::clear_app_name() {
  if (app_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_->clear();
  }
  clear_has_app_name();
}
inline const ::std::string& RunningAppItem::app_name() const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.RunningAppItem.app_name)
  return *app_name_;
}
inline void RunningAppItem::set_app_name(const ::std::string& value) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(value);
  // @@protoc_insertion_point(field_set:com.eightplus.appinfo.RunningAppItem.app_name)
}
inline void RunningAppItem::set_app_name(const char* value) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.eightplus.appinfo.RunningAppItem.app_name)
}
inline void RunningAppItem::set_app_name(const char* value, size_t size) {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  app_name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.eightplus.appinfo.RunningAppItem.app_name)
}
inline ::std::string* RunningAppItem::mutable_app_name() {
  set_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    app_name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.RunningAppItem.app_name)
  return app_name_;
}
inline ::std::string* RunningAppItem::release_app_name() {
  clear_has_app_name();
  if (app_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = app_name_;
    app_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RunningAppItem::set_allocated_app_name(::std::string* app_name) {
  if (app_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete app_name_;
  }
  if (app_name) {
    set_has_app_name();
    app_name_ = app_name;
  } else {
    clear_has_app_name();
    app_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.eightplus.appinfo.RunningAppItem.app_name)
}

// required string package_name = 2;
inline bool RunningAppItem::has_package_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RunningAppItem::set_has_package_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RunningAppItem::clear_has_package_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RunningAppItem::clear_package_name() {
  if (package_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_->clear();
  }
  clear_has_package_name();
}
inline const ::std::string& RunningAppItem::package_name() const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.RunningAppItem.package_name)
  return *package_name_;
}
inline void RunningAppItem::set_package_name(const ::std::string& value) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(value);
  // @@protoc_insertion_point(field_set:com.eightplus.appinfo.RunningAppItem.package_name)
}
inline void RunningAppItem::set_package_name(const char* value) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(value);
  // @@protoc_insertion_point(field_set_char:com.eightplus.appinfo.RunningAppItem.package_name)
}
inline void RunningAppItem::set_package_name(const char* value, size_t size) {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  package_name_->assign(reinterpret_cast<const char*>(value), size);
  // @@protoc_insertion_point(field_set_pointer:com.eightplus.appinfo.RunningAppItem.package_name)
}
inline ::std::string* RunningAppItem::mutable_package_name() {
  set_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    package_name_ = new ::std::string;
  }
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.RunningAppItem.package_name)
  return package_name_;
}
inline ::std::string* RunningAppItem::release_package_name() {
  clear_has_package_name();
  if (package_name_ == &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    return NULL;
  } else {
    ::std::string* temp = package_name_;
    package_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
    return temp;
  }
}
inline void RunningAppItem::set_allocated_package_name(::std::string* package_name) {
  if (package_name_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete package_name_;
  }
  if (package_name) {
    set_has_package_name();
    package_name_ = package_name;
  } else {
    clear_has_package_name();
    package_name_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  }
  // @@protoc_insertion_point(field_set_allocated:com.eightplus.appinfo.RunningAppItem.package_name)
}

// -------------------------------------------------------------------

// RunningAppList

// repeated .com.eightplus.appinfo.RunningAppItem item = 1;
inline int RunningAppList::item_size() const {
  return item_.size();
}
inline void RunningAppList::clear_item() {
  item_.Clear();
}
inline const ::com::eightplus::appinfo::RunningAppItem& RunningAppList::item(int index) const {
  // @@protoc_insertion_point(field_get:com.eightplus.appinfo.RunningAppList.item)
  return item_.Get(index);
}
inline ::com::eightplus::appinfo::RunningAppItem* RunningAppList::mutable_item(int index) {
  // @@protoc_insertion_point(field_mutable:com.eightplus.appinfo.RunningAppList.item)
  return item_.Mutable(index);
}
inline ::com::eightplus::appinfo::RunningAppItem* RunningAppList::add_item() {
  // @@protoc_insertion_point(field_add:com.eightplus.appinfo.RunningAppList.item)
  return item_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::com::eightplus::appinfo::RunningAppItem >&
RunningAppList::item() const {
  // @@protoc_insertion_point(field_list:com.eightplus.appinfo.RunningAppList.item)
  return item_;
}
inline ::google::protobuf::RepeatedPtrField< ::com::eightplus::appinfo::RunningAppItem >*
RunningAppList::mutable_item() {
  // @@protoc_insertion_point(field_mutable_list:com.eightplus.appinfo.RunningAppList.item)
  return &item_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace appinfo
}  // namespace eightplus
}  // namespace com

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_AppSendInfo_2eproto__INCLUDED
