package com.iflytek.utils;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.HashMap;
import java.util.List;

public class ReflectHelper {
    static HashMap<CacheKey, HashMap<String, Field>> sfiledCacheHashMap = new HashMap<CacheKey, HashMap<String, Field>>();
    static HashMap<CacheKey, HashMap<String, Field>> sfiledStaticCacheHashMap = new HashMap<CacheKey, HashMap<String, Field>>();

    static class CacheKey {
        Class<?> cls;

        public CacheKey(Class<?> cls) {
            this.cls = cls;

        }

        @Override
        public int hashCode() {
            return (cls.getClass().getName()).hashCode();
        }

        @Override
        public boolean equals(Object o) {
            if (o instanceof CacheKey) {
                CacheKey that = (CacheKey) o;
                if (this == o) {
                    return true;
                }
                if (that.cls == null || this.cls == null) {
                    return false;
                }
                if (this.cls.getName().equals(that.cls.getName())) {
                    return true;
                }
            }
            return false;
        }
    }

    public static HashMap<String, Field> getFields(Object object) {
        Class<?> cls = object.getClass();
        CacheKey cacheKey = new CacheKey(cls);
        if (sfiledCacheHashMap.containsKey(cacheKey)) {
            return sfiledCacheHashMap.get(cacheKey);
        }

        HashMap<String, Field> retFields = new HashMap<String, Field>();
        while (cls != Object.class) {
            Field[] fields = cls.getDeclaredFields();
            for (Field field : fields) {
                try {
                    if ((field.getModifiers() & Modifier.STATIC) != Modifier.STATIC) {
                        field.setAccessible(true);
                        retFields.put(field.getName(), field);
                    }
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                }
            }
            cls = cls.getSuperclass();
        }

        sfiledCacheHashMap.put(cacheKey, retFields);
        return retFields;
    }

    // 得到对应类的静态字段并且缓存起来
    public static HashMap<String, Field> getStaticFields(Class<?> cls) {
        CacheKey cacheKey = new CacheKey(cls);
        if (sfiledStaticCacheHashMap.containsKey(cacheKey)) {
            return sfiledStaticCacheHashMap.get(cacheKey);
        }

        HashMap<String, Field> retFields = new HashMap<String, Field>();
        while (cls != Object.class) {
            Field[] fields = cls.getDeclaredFields();
            for (Field field : fields) {
                try {
                    if ((field.getModifiers() & Modifier.STATIC) == Modifier.STATIC) {
                        field.setAccessible(true);
                        retFields.put(field.getName(), field);
                    }
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                }
            }
            cls = cls.getSuperclass();
        }

        sfiledStaticCacheHashMap.put(cacheKey, retFields);
        return retFields;
    }

    // 得到对应类的所有非静态字段并且缓存起来
    public static HashMap<String, Field> getFields(Class<?> cls) {
        CacheKey cacheKey = new CacheKey(cls);
        if (sfiledCacheHashMap.containsKey(cacheKey)) {
            return sfiledCacheHashMap.get(cacheKey);
        }

        HashMap<String, Field> retFields = new HashMap<String, Field>();
        while (cls != Object.class) {
            Field[] fields = cls.getDeclaredFields();
            for (Field field : fields) {
                try {
                    if ((field.getModifiers() & Modifier.STATIC) != Modifier.STATIC) {
                        field.setAccessible(true);
                        retFields.put(field.getName(), field);
                    }
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                }
            }
            cls = cls.getSuperclass();
        }

        sfiledCacheHashMap.put(cacheKey, retFields);
        return retFields;
    }

    // 实例化对象
    public static <T> T newInstance(Class<T> cls) {
        Constructor cons[] = cls.getDeclaredConstructors();
        if (cons.length > 0) {
            cons[0].setAccessible(true);

            try {
                return (T) cons[0].newInstance();
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public static <T> T newInstance(Class<T> cls, Object... args) {
        Constructor cons[] = cls.getDeclaredConstructors();
        if (cons.length > 0) {
            cons[0].setAccessible(true);

            try {
                return (T) cons[0].newInstance(args);
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            } catch (InstantiationException e) {
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    // 反射调用方法
    public static Object invoke(Class<?> cls, String methodName) {
        try {
            Method method = cls.getMethod(methodName);
            return method.invoke(cls);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static Object invoke(Object instance, String methodName) {
        try {
            Method method = instance.getClass().getMethod(methodName);
            return method.invoke(instance);
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static Object invoke(Object instance, Method method) {
        try {
            return method.invoke(instance);
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static Object invoke(Object instance, String methodName, Object... arg) {
        try {

            Method[] obj = instance.getClass().getMethods();
            for (Method method : obj) {
                if (methodName != null && methodName.equals(method.getName())) {
                    //如果是private修饰符的，则把可访问性设置为true
                    if (!method.isAccessible()) {
                        method.setAccessible(true);
                    }
                    //得到方法中的所有参数信息
                    Class<?>[] parameterClazz = method.getParameterTypes();

                    return method.invoke(instance, arg);
                }
            }

        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static Method[] getMethods(Class<?> cls) {
        return cls.getMethods();
    }

    private static void fillList(List<Object> list, Class<?> parameter, Object value) {
        switch (parameter.getName()) {
            case "java.lang.Character":
            case "char":
                char[] ch = ((String) value).toCharArray();
                list.add(ch[0]);
                break;
            case "java.lang.Double":
            case "double":
                list.add(Double.parseDouble((String) value));
                break;
            case "java.lang.Integer":
            case "int":
                list.add(Integer.parseInt((String) value));
                break;
            case "java.lang.Long":
            case "long":
                list.add(Long.parseLong((String) value));
                break;
            case "java.lang.Float":
            case "float":
                list.add(Float.parseFloat((String) value));
                break;
            case "java.lang.Short":
            case "shrot":
                list.add(Short.parseShort((String) value));
                break;
            case "java.lang.Byte":
            case "byte":
                list.add(Byte.parseByte((String) value));
                break;
            case "java.lang.Boolean":
            case "boolean":
                if ("false".equals(value) || "0".equals(value)) {
                    list.add(false);
                } else if ("true".equals(value) || "1".equals(value)) {
                    list.add(true);
                }
                break;
            default:
                list.add(value);
                break;
        }
    }

}
