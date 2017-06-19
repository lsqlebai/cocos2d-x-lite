# Add project specific ProGuard rules here.
# By default, the flags in this file are appended to flags specified
# in E:\developSoftware\Android\SDK/tools/proguard/proguard-android.txt
# You can edit the include path and order by changing the proguardFiles
# directive in build.gradle.
#
# For more details, see
#   http://developer.android.com/guide/developing/tools/proguard.html

# Add any project specific keep options here:

# If your project uses WebView with JS, uncomment the following
# and specify the fully qualified class name to the JavaScript interface
# class:
#-keepclassmembers class fqcn.of.javascript.interface.for.webview {
#   public *;
#}
-renamesourcefileattribute SourceFile
-keepattributes Signature, *Annotation*, SourceFile, LineNumberTable
-ignorewarnings
-optimizationpasses 5
-dontusemixedcaseclassnames
-dontskipnonpubliclibraryclasses
-dontpreverify
-verbose

#ubp
-keep class com.iflytek.pay.** {*;}
-dontwarn com.iflytek.pay.**
-keep class com.iflytek.payweixin.** {*;}
-keep class com.iflytek.weixin.** {*;}
-keep class com.iflytek.alipay.** {*;}

#unicom
-keep class com.huawei.**{*;}
-keep class org.kxml2.**{*;}
-keep class org.xmlpull.**{*;}
-keep class org.tantalum.**{*;}
-keep class com.sdk.commplatform.**{*;}
-keep class assets.**{*;}
-keep class res.**{*;}


#xiaowo sdk
-keep class com.vcheng.** {*;}

#widget
-keep class com.iflytek.musicnb.widget.FocusHighlightLayout2$HighlightImageView {*;}

#pay
-keep class com.iflytek.payComponent.**{*;}
-keep class com.iflytek.unipay.**{*;}

#umeng
-keep public class com.iflytek.leagueofglutton.R$*{ public static final int *; }
-keepclassmembers class * { public (org.json.JSONObject); }
-keepclassmembers enum * { public static **[] values(); public static ** valueOf(java.lang.String); }