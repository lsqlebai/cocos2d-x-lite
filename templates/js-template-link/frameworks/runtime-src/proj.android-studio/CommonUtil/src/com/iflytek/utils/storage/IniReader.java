package com.iflytek.utils.storage;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

/**
 * Created by wpfang on 14-6-30.
 */
public class IniReader {

    private Map<String, Properties> sections;
    private Properties properties;

    public IniReader(String filename) throws IOException {
        sections = new HashMap<String, Properties>();
        BufferedReader reader = new BufferedReader(new FileReader(filename));
        read(reader);
        reader.close();
    }

    private void read(BufferedReader reader) throws IOException {
        String line;
        while ((line = reader.readLine()) != null) {
            parseLine(line);
        }
    }

    private void parseLine(String line) {
        line = line.trim();
        if (line.matches("\\[.*\\]")) {
            String secion = line.replaceFirst("\\[(.*)\\]", "$1");
            properties = new Properties();
            sections.put(secion, properties);
        } else if (line.matches(".*=.*")) {
            if (properties != null) {
                int i = line.indexOf('=');
                String name = line.substring(0, i);
                String value = line.substring(i + 1);
                properties.setProperty(name, value);
            }
        }
    }

    public String getValue(String section, String name) {
        Properties p = sections.get(section);
        if (p == null) {
            return null;
        }
        String value = p.getProperty(name);
        return value;
    }
}
