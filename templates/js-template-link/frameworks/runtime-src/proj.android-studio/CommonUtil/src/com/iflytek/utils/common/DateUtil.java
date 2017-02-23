package com.iflytek.utils.common;

import com.iflytek.log.Logger;
import com.iflytek.utils.string.StringUtil;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.Locale;

/**
 * Title:时间工具类
 * @author wpfang
 * @version 1.0
 */
public class DateUtil {
    /**
     * 将字符串转化为DATE
     *
     * @param dtFormat 格式yyyy-MM-dd HH:mm:ss 或 yyyy-MM-dd或 yyyy-M-dd或 yyyy-M-d或
     *                 yyyy-MM-d或 yyyy-M-dd
     * @param def      如果格式化失败返回null
     * @return
     */
    public static Date fmtStrToDate(String dtFormat) {
        if (dtFormat == null) {
            return null;
        }
        try {
            if (dtFormat.length() == 9 || dtFormat.length() == 8) {
                String[] dateStr = dtFormat.split("-");
                dtFormat = dateStr[0] + (dateStr[1].length() == 1 ? "-0" : "-")
                        + dateStr[1] + (dateStr[2].length() == 1 ? "-0" : "-")
                        + dateStr[2];
            }
            if (dtFormat.length() != 10 & dtFormat.length() != 19)
                return null;
            if (dtFormat.length() == 10)
                dtFormat = dtFormat + " 00:00:00";
            SimpleDateFormat dateFormat = new SimpleDateFormat(
                    "yyyy-MM-dd HH:mm:ss");
            return dateFormat.parse(dtFormat);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
    


    /**
     * Description:格式化日期,如果格式化失败返回def
     *
     * @param dtFormat
     * @param def
     * @return
     */
    public static Date fmtStrToDate(String dtFormat, Date def) {
        Date d = fmtStrToDate(dtFormat);
        if (d == null)
            return def;
        return d;
    }
    
    /**
     * 转换成MM:SS
     * @param msec 歌曲总时间长度(毫秒)
     * @return
     */
    public static String formatMSTime(int msec) {
		int min = msec / 60000;
		int sec = msec / 1000 % 60;
		return String.format(Locale.US, "%02d:%02d", min, sec);
	}

    /**
     * 返回当日短日期型
     *
     * @return
     */
    public static Date getToDay() {
        return toShortDate(new Date());
    }

    /**
     * Description:格式化日期,String字符串转化为Date
     *
     * @param date
     * @param dtFormat 例如:yyyy-MM-dd HH:mm:ss yyyyMMdd
     * @return
     */
    public static String fmtDateToStr(Date date, String dtFormat) {
        if (date == null)
            return "";
        try {
            SimpleDateFormat dateFormat = new SimpleDateFormat(dtFormat);
            return dateFormat.format(date);
        } catch (Exception e) {
            e.printStackTrace();
            return "";
        }
    }

    /**
     * Description:按指定格式 格式化日期
     *
     * @param date
     * @param dtFormat
     * @return
     */
    public static Date fmtStrToDate(String date, String dtFormat) {
        try {
            SimpleDateFormat dateFormat = new SimpleDateFormat(dtFormat);
            return dateFormat.parse(date);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    public static String fmtDateToYMDHM(Date date) {
        return fmtDateToStr(date, "yyyy-MM-dd HH:mm");
    }

    public static String fmtDateToYMD(Date date) {
        return fmtDateToStr(date, "yyyy-MM-dd");
    }

    public static String fmtDateToYM(Date date) {
        return fmtDateToStr(date, "yyyy-MM");
    }

    public static String fmtDateToM(Date date) {
        return fmtDateToStr(date, "MM");
    }
    
    /**
     * Description:只保留日期中的年月日
     *
     * @param date
     * @return
     */
    public static Date toShortDate(Date date) {
        String strD = fmtDateToStr(date, "yyyy-MM-dd");
        return fmtStrToDate(strD);
    }

    /**
     * Description:只保留日期中的年月日
     *
     * @paramdate格式要求 yyyy-MM-dd
     * @return
     */
    public static Date toShortDate(String date) {
        if (date != null && date.length() >= 10) {
            return fmtStrToDate(date.substring(0, 10));
        } else
            return fmtStrToDate(date);
    }

    /**
     * 求对日
     *
     * @param countMonth :月份的个数(几个月)
     * @param flag:true 求前countMonth个月的对日:false 求下countMonth个月的对日
     * @return
     */
    public static Date getCounterglow(int countMonth, boolean before) {
        Calendar ca = Calendar.getInstance();
        return getCounterglow(ca.getTime(), before ? -countMonth : countMonth);
    }

    /**
     * Description: 求对日 加月用+ 减月用-
     *
     * @param date
     * @param countMonth
     * @return
     */
    public static Date getCounterglow(Date date, int num) {
        Calendar ca = Calendar.getInstance();
        ca.setTime(date);
        ca.add(Calendar.MONTH, num);
        return ca.getTime();
    }

    /**
     * Description:加一天
     *
     * @param date
     * @return
     */
    public static Date addDay(Date date) {
        Calendar cd = Calendar.getInstance();
        cd.setTime(date);
        cd.add(Calendar.DAY_OF_YEAR, 1);
        return cd.getTime();
    }

    /**
     * Description:判断一个日期是否为工作日(非周六周日)
     *
     * @param date
     * @return
     */
    public static boolean isWorkDay(Date date) {
        Calendar cd = Calendar.getInstance();
        cd.setTime(date);
        int dayOfWeek = cd.get(Calendar.DAY_OF_WEEK);
        return !(dayOfWeek != Calendar.SUNDAY || dayOfWeek != Calendar.SATURDAY);
    }

    /**
     * Description:取一个月的最后一天
     *
     * @param date1
     * @return
     */
    public static Date getLastDayOfMonth(Date date1) {
        Calendar date = Calendar.getInstance();
        date.setTime(date1);
        date.set(Calendar.DAY_OF_MONTH, 1);
        date.add(Calendar.MONTH, 1);
        date.add(Calendar.DAY_OF_YEAR, -1);
        return toShortDate(date.getTime());
    }

    /**
     * 求开始截至日期之间的天数差.
     *
     * @param d1 开始日期
     * @param d2 截至日期
     * @return 返回相差天数
     */
    public static int getDaysInterval(Date d1, Date d2) {
        if (d1 == null || d2 == null)
            return 0;
        Date[] d = new Date[2];
        d[0] = d1;
        d[1] = d2;
        Calendar[] cal = new Calendar[2];
        for (int i = 0; i < cal.length; i++) {
            cal[i] = Calendar.getInstance();
            cal[i].setTime(d[i]);
            cal[i].set(Calendar.HOUR_OF_DAY, 0);
            cal[i].set(Calendar.MINUTE, 0);
            cal[i].set(Calendar.SECOND, 0);
        }
        long m = cal[0].getTime().getTime();
        long n = cal[1].getTime().getTime();
        int ret = (int) Math.abs((m - n) / 1000 / 3600 / 24);
        return ret;
    }

    public static String getDayOfWeek(Date date) {
        Calendar cl = Calendar.getInstance();
        cl.setTime(date);
        return "周" + toChNumber(cl.get(Calendar.DAY_OF_WEEK) - 1);
    }

    /**
     * 将数字转为中文。 "0123456789"->"〇一二三四五六七八九"
     *
     * @param num 长度为1,'0'-'9'的字符串
     * @return
     */
    private static String toChNumber(int num) {
        final String str = "〇一二三四五六七八九";
        return str.substring(num, num + 1);
    }

    /**
     * Description:指定日期加或减days天
     *
     * @param date1日期
     * @param days天数
     * @return
     */
    public static Date addDay(Date date1, int days) {
        Calendar date = Calendar.getInstance();
        date.setTime(date1);
        date.add(Calendar.DAY_OF_YEAR, days);
        return date.getTime();
    }

    /**
     * Description:指定日期加或减months月
     *
     * @param date1
     * @param months
     * @return
     */
    public static Date addMonth(Date date1, int months) {
        Calendar date = Calendar.getInstance();
        date.setTime(date1);
        date.add(Calendar.MONTH, months);
        return date.getTime();
    }

    /**
     * Description:指定日期加或减years年
     *
     * @param date1
     * @param years
     * @return
     */
    public static Date addYear(Date date1, int years) {
        Calendar date = Calendar.getInstance();
        date.setTime(date1);
        date.add(Calendar.YEAR, years);
        return date.getTime();
    }

    /**
     * 指定期间的开始日期
     *
     * @param date 指定日期
     * @param type 期间类型
     * @param diff 与指定日期的范围
     * @return
     */
    public static Date getPeriodStart(Calendar date, int type, int diff) {
        date.add(type, diff * (-1));
        return date.getTime();
    }

    /**
     * 指定期间的开始日期
     *
     * @param date 指定日期
     * @param type 期间类型
     * @param diff 与指定日期的范围
     * @return
     */
    public static Date getPeriodStart(Date date, int type, int diff) {
        return getPeriodStart(dateToCalendar(date), type, diff);
    }

    /**
     * 指定期间的结束日期
     *
     * @param date 指定日期
     * @param type 期间类型
     * @param diff 与指定日期的范围
     * @return
     */
    public static Date getPeriodEnd(Calendar date, int type, int diff) {
        date.add(type, diff);
        return date.getTime();
    }

    /**
     * 指定期间的结束日期
     *
     * @param date 指定日期
     * @param type 期间类型
     * @param diff 与指定日期的范围
     * @return
     */
    public static Date getPeriodEnd(Date date, int type, int diff) {
        return getPeriodEnd(dateToCalendar(date), type, diff);
    }

    /**
     * 指定日期所在星期的第一天
     *
     * @param date
     * @return
     */
    public static Date getWeekStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        cdate.set(Calendar.DAY_OF_WEEK, 2);
        return cdate.getTime();
    }

    /**
     * 将java.util.Date类型转换成java.util.Calendar类型
     *
     * @param date
     * @return
     */
    public static Calendar dateToCalendar(Date date) {
        Calendar cdate = Calendar.getInstance();
        cdate.setTime(date);
        return cdate;
    }

    /**
     * 指定日期所在月的第一天
     *
     * @param date
     * @return
     */
    public static Date getMonthStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        cdate.set(Calendar.DAY_OF_MONTH, 1);
        return toShortDate(cdate.getTime());
    }

    /**
     * 指定日期所在上月的第一天
     *
     * @param date
     * @return
     */
    public static Date getLastMonthStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        cdate.set(Calendar.DAY_OF_MONTH, 1);
        cdate.add(Calendar.MONTH, -1);
        return toShortDate(cdate.getTime());
    }

    /**
     * 指定日期所在旬的第一天
     *
     * @param date
     * @return
     */
    public static Date getTenDaysStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        int day = cdate.get(Calendar.DAY_OF_MONTH) / 10 * 10 + 1;
        if (cdate.get(Calendar.DAY_OF_MONTH) % 10 == 0 || day == 31)
            day = day - 10;
        cdate.set(Calendar.DAY_OF_MONTH, day);
        return cdate.getTime();
    }

    /**
     * 指定日期所在旬的最后一天
     *
     * @param date
     * @return
     */
    public static Date getTenDaysEnd(Date date) {
        Calendar cdate = dateToCalendar(date);
        if (cdate.get(Calendar.DAY_OF_MONTH) / 10 == 2
                && cdate.get(Calendar.DAY_OF_MONTH) != 20)
            return getLastDayOfMonth(date);
        else
            return addDay(getTenDaysStart(addDay(date, 10)), -1);
    }

    /**
     * 指定日期所在年的第一天
     *
     * @param date
     * @return
     */
    public static Date getYearStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        cdate.set(Calendar.DAY_OF_YEAR, 1);
        return cdate.getTime();
    }

    /**
     * 指定日期所在季度的第一天
     *
     * @param date
     * @return
     */
    public static Date getQuarterStart(Date date) {
        Calendar cdate = dateToCalendar(date);
        int month = (cdate.get(Calendar.MONTH) / 3) * 3;
        cdate.set(Calendar.MONTH, month);
        return getMonthStart(cdate.getTime());
    }

    /**
     * 指定日期返回带中文的字符串（目前为年月日类型，之后补充）
     *
     * @param date
     * @param format
     * @return
     */
    public static String dateToStringByChinese(String format, Date date) {
        String dateString = fmtDateToStr(date, format);
        String[] dateStringArray = dateString.split("-");
        if ("yyyy-MM-dd".equals(format)) {
            dateString = dateStringArray[0] + "年" + dateStringArray[1] + "月"
                    + dateStringArray[2] + "日";
        } else if ("yyyy-MM".equals(format)) {
            dateString = dateStringArray[0] + "年" + dateStringArray[1] + "月";
        }
        return dateString;
    }

    public static Date getLastDayOfYear(Date date) {
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy");
        String years = dateFormat.format(date);
        years += "-12-31";
        Date returnDate = fmtStrToDate(years);
        return toShortDate(returnDate);
    }

    /**
     * 计算两个日期之间相差的月数
     *
     * @param date1
     * @param date2
     * @return
     */
    public static int getMonths(Date date1, Date date2) {
        int iMonth = 0;
        int flag = 0;
        try {
            Calendar objCalendarDate1 = Calendar.getInstance();
            objCalendarDate1.setTime(date1);

            Calendar objCalendarDate2 = Calendar.getInstance();
            objCalendarDate2.setTime(date2);

            if (objCalendarDate2.equals(objCalendarDate1))
                return 0;
            if (objCalendarDate1.after(objCalendarDate2)) {
                Calendar temp = objCalendarDate1;
                objCalendarDate1 = objCalendarDate2;
                objCalendarDate2 = temp;
            }
            if (objCalendarDate2.get(Calendar.DAY_OF_MONTH) < objCalendarDate1
                    .get(Calendar.DAY_OF_MONTH))
                flag = 1;

            if (objCalendarDate2.get(Calendar.YEAR) > objCalendarDate1
                    .get(Calendar.YEAR))
                iMonth = ((objCalendarDate2.get(Calendar.YEAR) - objCalendarDate1
                        .get(Calendar.YEAR))
                        * 12 + objCalendarDate2.get(Calendar.MONTH) - flag)
                        - objCalendarDate1.get(Calendar.MONTH);
            else
                iMonth = objCalendarDate2.get(Calendar.MONTH)
                        - objCalendarDate1.get(Calendar.MONTH) - flag;

        } catch (Exception e) {
            e.printStackTrace();
        }
        return iMonth;
    }

    /**
     * 指定日期上一个旬的第一天
     */
    public static Date getLastTenStartDate(Date date) {
        Date returnDate = DateUtil.toShortDate(date);
        returnDate = DateUtil.getTenDaysStart(date);
        returnDate = DateUtil.addDay(returnDate, -1);
        returnDate = DateUtil.getTenDaysStart(returnDate);
        return DateUtil.toShortDate(returnDate);
    }

    /**
     * 指定日期上一个旬的最后一天
     */
    public static Date getLastTenEndDate(Date date) {
        Date returnDate = DateUtil.toShortDate(date);
        returnDate = DateUtil.getTenDaysStart(date);
        returnDate = DateUtil.addDay(returnDate, -1);
        return DateUtil.toShortDate(returnDate);
    }

    /**
     * 指定日期上个月第一天
     */
    public static Date getLastMonthStartDate(Date date) {
        Date returnDate = DateUtil.toShortDate(date);
        returnDate = DateUtil.getLastMonthStart(date);
        return DateUtil.toShortDate(returnDate);
    }

    /**
     * 指定日期上个月最后一天
     */
    public static Date getLastMonthEndDate(Date date) {
        Date returnDate = DateUtil.toShortDate(date);
        returnDate = DateUtil.getMonthStart(date);
        returnDate = DateUtil.addDay(returnDate, -1);
        return DateUtil.toShortDate(returnDate);
    }
    
    /**
     * 获取年龄
     * @param birthday 出生日期
     * @param nowDate 当前时间
     * @return 周岁，舍去余数
     */
    public static int getAge(Date birthday,Date nowDate){
    	long days= (nowDate.getTime() - birthday.getTime())/(1000*60*60*24);//得到总天数
    	int years=(int) (days/365);//周岁
    	return years;
    }

    public static final String[] constellationArr = { "水瓶座", "双鱼座", "白羊座", "金牛座", "双子座", "巨蟹座", "狮子座", "处女座", "天秤座",    
        "天蝎座", "射手座", "魔羯座" };    
    public static final int[] constellationEdgeDay = { 20, 19, 21, 21, 21, 22, 23, 23, 23, 23, 22, 22 };    

    /**   
     * 根据日期获取星座   
     * @param time   
     * @return   
     */   
    public static String date2Constellation(Date time) {    
        int month = time.getMonth();    
        int day = time.getDate();    
        if (day < constellationEdgeDay[month]) {    
            month = month - 1;    
        }    
        if (month >= 0) {    
            return constellationArr[month];    
        }    
        //default to return 魔羯    
        return constellationArr[11];    
    }

    /**
     *
     * @param nowTime 当前时间
     * @param outofdate 上限时间
     * @return 是否过期
     */
    public static boolean isOutOfTime(Date nowTime,String outofdate){

        if(StringUtil.isEmpty(outofdate)){
            return true;
        }
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        try
        {
            Date d2 = df.parse(outofdate);
            return nowTime.getTime() - d2.getTime() >0;
        }
        catch (Exception e)
        {
        }
        return true;

    }

    /**
     *
     * @param nowTime 当前时间
     * @param outofdate 上限时间
     * @return 是否过期
     */
    public static boolean isOutOfTime(String nowTime,String outofdate){
        DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        try
        {
            Date d1 = df.parse(nowTime);
            Date d2 = df.parse(outofdate);
            Logger.log().e("d1 "+d1 +" d2:" +d2 );
            return d1.getTime() - d2.getTime() > 0;
        }
        catch (Exception e)
        {
        }
        return true;

    }
}
