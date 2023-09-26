#include "lidar_for_kotlin/utils.hpp"

void throwException(JNIEnv * env, std::string message)
{
  env->ExceptionDescribe();
  env->ExceptionClear();

  const char * class_name = "java/lang/IllegalStateException";
  jclass exception_class = env->FindClass(class_name);
  assert(exception_class != NULL);

  env->ThrowNew(exception_class, message.c_str());
}

void jniExceptionCheck(JNIEnv * env)
{
  if (env->ExceptionCheck() == JNI_TRUE) {
    throwException(env, "CallNativeObject");
  }
}


std::string jstring2String(JNIEnv * env, jstring jsubject)
{
  const char * subject_tmp = env->GetStringUTFChars(jsubject, 0);
  std::string result(subject_tmp);
  env->ReleaseStringUTFChars(jsubject, subject_tmp);

  return result;
}

char ** JniStringArray2StringArray(JNIEnv * env, jobjectArray stringArray)
{
  jsize stringCount = env->GetArrayLength(stringArray);
  char ** strings = reinterpret_cast<char **>(malloc(sizeof(char *) * stringCount));

  int i = 0;
  for (i = 0; i < stringCount; ++i) {
    jstring jniString = (jstring) env->GetObjectArrayElement(stringArray, i);
    const char * TempString = env->GetStringUTFChars(jniString, NULL);
    strings[i] = const_cast<char *>(TempString);
    env->ReleaseStringUTFChars(jniString, TempString);
  }

  return strings;
}

/*
 * Convertion of Message type.
 */
rosidl_message_type_support_t *
jclass2MessageType(JNIEnv * env, jclass jmessage_class)
{
  jmethodID mid = env->GetStaticMethodID(jmessage_class, "getTypeSupport", "()J");
  jniExceptionCheck(env);
  assert(mid != NULL);

  jlong jts = env->CallStaticLongMethod(jmessage_class, mid);
  jniExceptionCheck(env);
  assert(jts != 0);

  rosidl_message_type_support_t * ts =
    reinterpret_cast<rosidl_message_type_support_t *>(jts);

  return ts;
}

/*
 *
 */
rosidl_service_type_support_t *
jclass2ServiceType(JNIEnv * env, jclass jmessage_class)
{
  jmethodID mid = env->GetStaticMethodID(jmessage_class, "getServiceTypeSupport", "()J");
  jniExceptionCheck(env);
  assert(mid != NULL);

  jlong jts = env->CallStaticLongMethod(jmessage_class, mid);
  jniExceptionCheck(env);
  assert(jts != 0);

  rosidl_service_type_support_t * ts =
    reinterpret_cast<rosidl_service_type_support_t *>(jts);

  return ts;
}

/*
 *
 */
void *
jclass2Message(JNIEnv * env, jclass jmessage_class)
{
  jmethodID jfrom_mid = env->GetStaticMethodID(jmessage_class, "getFromJavaConverter", "()J");
  assert(jfrom_mid != NULL);

  jlong jfrom_java_converter = env->CallStaticLongMethod(jmessage_class, jfrom_mid);
  jniExceptionCheck(env);
  assert(jfrom_java_converter != 0);

  jmethodID jconstructor = env->GetMethodID(jmessage_class, "<init>", "()V");
  assert(jconstructor != NULL);

  jobject jmsg = env->NewObject(jmessage_class, jconstructor);
  assert(jmsg != NULL);

  using convert_from_java_signature = void * (*)(jobject, void *);
  convert_from_java_signature convert_from_java =
    reinterpret_cast<convert_from_java_signature>(jfrom_java_converter);

  void * taken_msg = convert_from_java(jmsg, nullptr);
  if (taken_msg == NULL) {
    throwException(env, "getFromJavaConverter");
  }

  return taken_msg;
}

jobject
jclass2JMessage(JNIEnv * env, jclass jmessage_class, void * taken_msg)
{
  jmethodID jto_mid = env->GetStaticMethodID(jmessage_class, "getToJavaConverter", "()J");
  assert(jto_mid != NULL);

  jlong jto_java_converter = env->CallStaticLongMethod(jmessage_class, jto_mid);
  jniExceptionCheck(env);
  assert(jto_java_converter != 0);

  using convert_to_java_signature = jobject (*)(void *, jobject);
  convert_to_java_signature convert_to_java =
    reinterpret_cast<convert_to_java_signature>(jto_java_converter);

  jobject jtaken_msg = convert_to_java(taken_msg, nullptr);
  if (jtaken_msg == NULL) {
    throwException(env, "getFromJavaConverter");
  }

  return jtaken_msg;
}

/*
 *
 */
void *
jobject2Message(JNIEnv * env, jobject jmessage)
{
  jclass jmessage_class = env->GetObjectClass(jmessage);
  assert(jmessage_class != NULL);

  jmethodID mid = env->GetStaticMethodID(jmessage_class, "getFromJavaConverter", "()J");
  assert(mid != NULL);

  jlong jfrom_java_converter = env->CallStaticLongMethod(jmessage_class, mid);
  jniExceptionCheck(env);
  assert(jfrom_java_converter != 0);

  using convert_from_java_signature = void * (*)(jobject, void *);
  convert_from_java_signature convert_from_java =
    reinterpret_cast<convert_from_java_signature>(jfrom_java_converter);

  void * raw_ros_message = convert_from_java(jmessage, nullptr);
  if (raw_ros_message == NULL) {
    throwException(env, "getFromJavaConverter");
  }

  return raw_ros_message;
}

/*
 *
 */
jobject
makeJTopics(JNIEnv * env, rcl_names_and_types_t * topic_names_and_types)
{
  env->PushLocalFrame(256);  // fix for local references

  size_t map_len = (size_t)topic_names_and_types->names.size;
  jclass mapClass = env->FindClass("java/util/HashMap");
  jmethodID initMap = env->GetMethodID(mapClass, "<init>", "(I)V");
  jobject hashMap = env->NewObject(mapClass, initMap, map_len);
  jmethodID put = env->GetMethodID(mapClass, "put",
      "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");

  jclass listClass = env->FindClass("java/util/ArrayList");
  jmethodID initList = env->GetMethodID(listClass, "<init>", "(I)V");
  jmethodID add = env->GetMethodID(listClass, "add",
      "(Ljava/lang/Object;)Z");

  for (size_t i = 0; i < map_len; ++i) {
    size_t list_len = (size_t)topic_names_and_types->types[i].size;
    jobject arrayList = env->NewObject(listClass, initList, list_len);

    for (size_t j = 0; j < list_len; ++j) {
      env->CallObjectMethod(arrayList, add,
        env->NewStringUTF(topic_names_and_types->types[i].data[j]));
      jniExceptionCheck(env);
    }

    env->CallObjectMethod(hashMap, put,
      env->NewStringUTF(topic_names_and_types->names.data[i]),
      arrayList);
    jniExceptionCheck(env);
  }

  env->PopLocalFrame(hashMap);

  return hashMap;
}

jobject
makeJNodes(JNIEnv * env, rcutils_string_array_t * nodes)
{
  env->PushLocalFrame(256);  // fix for local references

  jsize list_len = nodes->size;
  jclass listClass = env->FindClass("java/util/ArrayList");
  jmethodID init = env->GetMethodID(listClass, "<init>", "(I)V");
  jobject arrayList = env->NewObject(listClass, init, list_len);
  jmethodID add = env->GetMethodID(listClass, "add",
      "(Ljava/lang/Object;)Z");

  for (size_t i = 0; i < nodes->size; ++i) {
    env->CallObjectMethod(arrayList, add,
      env->NewStringUTF(nodes->data[i]));
    jniExceptionCheck(env);
  }

  env->PopLocalFrame(arrayList);

  return arrayList;
}


jobject convert_rmw_request_id_to_java(JNIEnv * env, rmw_request_id_t * request_id)
{
  jclass jrequest_id_class = env->FindClass("org/ros2/rcljava/node/service/RMWRequestId");
  assert(jrequest_id_class != nullptr);

  jmethodID jconstructor = env->GetMethodID(jrequest_id_class, "<init>", "()V");
  assert(jconstructor != nullptr);

  jobject jrequest_id = env->NewObject(jrequest_id_class, jconstructor);

  jfieldID jsequence_number_field_id = env->GetFieldID(jrequest_id_class, "sequenceNumber", "J");
  jfieldID jwriter_guid_field_id = env->GetFieldID(jrequest_id_class, "writerGUID", "[B");

  assert(jsequence_number_field_id != nullptr);
  assert(jwriter_guid_field_id != nullptr);

  int8_t * writer_guid = request_id->writer_guid;
  int64_t sequence_number = request_id->sequence_number;

  env->SetLongField(jrequest_id, jsequence_number_field_id, sequence_number);

  jsize writer_guid_len = 16;  // See rmw/rmw/include/rmw/types.h

  jbyteArray jwriter_guid = env->NewByteArray(writer_guid_len);
  env->SetByteArrayRegion(jwriter_guid, 0, writer_guid_len, reinterpret_cast<jbyte *>(writer_guid));
  env->SetObjectField(jrequest_id, jwriter_guid_field_id, jwriter_guid);

  return jrequest_id;
}

rmw_request_id_t * convert_rmw_request_id_from_java(JNIEnv * env, jobject jrequest_id)
{
  assert(jrequest_id != nullptr);

  jclass jrequest_id_class = env->GetObjectClass(jrequest_id);
  assert(jrequest_id_class != nullptr);

  jfieldID jsequence_number_field_id = env->GetFieldID(jrequest_id_class, "sequenceNumber", "J");
  jfieldID jwriter_guid_field_id = env->GetFieldID(jrequest_id_class, "writerGUID", "[B");

  assert(jsequence_number_field_id != nullptr);
  assert(jwriter_guid_field_id != nullptr);

  rmw_request_id_t * request_id = static_cast<rmw_request_id_t *>(malloc(sizeof(rmw_request_id_t)));

  int8_t * writer_guid = request_id->writer_guid;
  request_id->sequence_number = env->GetLongField(jrequest_id, jsequence_number_field_id);

  jsize writer_guid_len = 16;  // See rmw/rmw/include/rmw/types.h

  jbyteArray jwriter_guid = (jbyteArray)env->GetObjectField(jrequest_id, jwriter_guid_field_id);
  env->GetByteArrayRegion(jwriter_guid, 0, writer_guid_len, reinterpret_cast<jbyte *>(writer_guid));

  return request_id;
}

/*
 *
 */
jlong
instance2Handle(void * obj)
{
  jlong handler = reinterpret_cast<jlong>(obj);
  assert(handler != 0);

  return handler;
}