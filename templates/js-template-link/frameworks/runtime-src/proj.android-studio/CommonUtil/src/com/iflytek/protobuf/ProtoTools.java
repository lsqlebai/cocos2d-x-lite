package com.iflytek.protobuf;

import com.iflytek.utils.ReflectHelper;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;

/**
 * Created by lsq on 2016/7/19.
 */
public class ProtoTools {
    public static <T> T fillProtoFields(T builder, Object object) {
        HashMap<String, Field> fields = ReflectHelper.getFields(object.getClass());
        for (String key : fields.keySet()) {
            Field field = fields.get(key);
            ProtoField protoField = field.getAnnotation(ProtoField.class);
            if (protoField != null) {
                String mappingField = protoField.value();
                if (mappingField.equals("")) {
                    mappingField = key;
                }
                ReflectHelper.invoke(builder, protoField.type() + upperCaseFirstChar(mappingField),
                        ReflectHelper.invoke(object, "get" + upperCaseFirstChar(key)));
            }


            ProtoListField protoList = field.getAnnotation(ProtoListField.class);
            if (protoList != null) {
                String mappingField = protoList.value();
                if (mappingField.equals("")) {
                    mappingField = key;
                }
                ReflectHelper.invoke(builder, "addAll" + upperCaseFirstChar(mappingField),
                        makeProtoList(
                                protoList.type(),
                                (List) ReflectHelper.invoke(object, "get" + upperCaseFirstChar(key)))
                );
            }

        }

        Method[] methods = ReflectHelper.getMethods(object.getClass());
        for (Method method : methods) {
            ProtoField protoField = method.getAnnotation(ProtoField.class);
            if (protoField != null) {
                String mappingField = protoField.value();
                if (mappingField.equals("")) {
                    mappingField = lowerCaseFirstChar(method.getName().replace("get", ""));
                }
                ReflectHelper.invoke(builder, protoField.type() + upperCaseFirstChar(mappingField),
                        ReflectHelper.invoke(object, method));

            }
        }

        return builder;
    }

    public static <T> T makeProtoObject(Class<T> protoClass, Object object) {
        Object builder = ReflectHelper.invoke(protoClass, "newBuilder");
        fillProtoFields(builder, object);
        return (T) ReflectHelper.invoke(builder, "build");
    }

    public static <T> List<T> makeProtoList(Class<T> protoClass, Collection<?> objects) {
        ArrayList<T> retList = new ArrayList<>();
        for (Object object : objects) {
            retList.add(makeProtoObject(protoClass, object));
        }
        return retList;
    }


    public static <T> T protoToPOJO(T pojo, Object proto) {
        HashMap<String, Field> fields = ReflectHelper.getFields(pojo.getClass());

        for (String key : fields.keySet()) {
            Field field = fields.get(key);
            ProtoField protoField = field.getAnnotation(ProtoField.class);
            if (protoField != null) {
                String mappingField = protoField.value();
                if (mappingField.equals("")) {
                    mappingField = key;
                }
                ReflectHelper.invoke(pojo, protoField.type() + upperCaseFirstChar(key),
                        ReflectHelper.invoke(proto, "get" + upperCaseFirstChar(mappingField)));
            }
            ProtoListField protoList = field.getAnnotation(ProtoListField.class);
            if (protoList != null) {
                String mappingField = protoList.value();
                if (mappingField.equals("")) {
                    mappingField = key;
                }
                ReflectHelper.invoke(pojo, "set" + upperCaseFirstChar(key),
                        makePOJOList(
                                protoList.type(),
                                (List<?>)ReflectHelper.invoke(proto, "get" + upperCaseFirstChar(mappingField) + "List"))
                );
            }
        }

        return pojo;
    }

    public static <T> List<T> makePOJOList(Class<T> pojoClass, Collection<?> objects) {
        ArrayList<T> retList = new ArrayList<>();
        for (Object object : objects) {
            retList.add(createPOJO(pojoClass, object));
        }
        return retList;
    }

    public static <T> T createPOJO(Class<T> pojoClass, Object proto) {
        T pojo = ReflectHelper.newInstance(pojoClass);
        if (pojo != null) {
            return protoToPOJO(pojo, proto);
        }
        return null;
    }


    private static String upperCaseFirstChar(String name) {
        //     name = name.substring(0, 1).toUpperCase() + name.substring(1);
//        return  name;
        char[] cs = name.toCharArray();
        cs[0] -= 32;
        return String.valueOf(cs);
    }

    private static String lowerCaseFirstChar(String name) {
        //     name = name.substring(0, 1).toUpperCase() + name.substring(1);
//        return  name;
        char[] cs = name.toCharArray();
        cs[0] += 32;
        return String.valueOf(cs);
    }
}
