#ifndef UTILS__HPP
#define UTILS__HPP

#include <jni.h>

#include <string>
#include <cstdlib>
#include <cassert>
#include <stdio.h>

#include "rcl/graph.h"
#include "rmw/rmw.h"
#include "rmw/types.h"
// #include "rosidl_generator_c/message_type_support_struct.h"


#ifdef __cplusplus
extern "C" {
#endif

void throwException(JNIEnv * env, std::string message);

void jniExceptionCheck(JNIEnv * env);

std::string jstring2String(JNIEnv * env, jstring jsubject);

char ** JniStringArray2StringArray(JNIEnv * env, jobjectArray stringArray);

rosidl_message_type_support_t * jclass2MessageType(JNIEnv * env, jclass jmessage_class);

rosidl_service_type_support_t * jclass2ServiceType(JNIEnv * env, jclass jmessage_class);

void * jclass2Message(JNIEnv * env, jclass jmessage_class);

jobject jclass2JMessage(JNIEnv * env, jclass jmessage_class, void * taken_msg);

void * jobject2Message(JNIEnv * env, jobject jmessage);

jobject makeJTopics(JNIEnv * env, rcl_names_and_types_t * topic_names_and_types);

jobject makeJNodes(JNIEnv * env, rcutils_string_array_t * nodes);

jobject convert_rmw_request_id_to_java(JNIEnv * env, rmw_request_id_t * request_id);

rmw_request_id_t * convert_rmw_request_id_from_java(JNIEnv * env, jobject jrequest_id);

jlong instance2Handle(void * obj);

#ifdef __cplusplus
}
#endif

template<typename T> T * makeInstance()
{
    T * obj = static_cast<T *>(malloc(sizeof(T)));
    assert(obj != NULL);

    return obj;
}

template<typename T> T * handle2Instance(jlong handle)
{
    T * obj = reinterpret_cast<T *>(handle);
    assert(obj != NULL);

    return obj;
}

#endif