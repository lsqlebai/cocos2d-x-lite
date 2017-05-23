package com.iflytek.utils.common;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.channels.FileChannel;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class FileUtil {
	public static void copyFile(String srcFile, String destFile)
			throws IOException {
		copyFile(new File(srcFile), new File(destFile));
	}

	public static void copyFile(File srcFile, File destFile) throws IOException {
		if (!destFile.exists()) {
			destFile.createNewFile();
		}

		FileChannel source = null;
		FileChannel destination = null;

		try {
			source = new FileInputStream(srcFile).getChannel();
			destination = new FileOutputStream(destFile).getChannel();
			destination.transferFrom(source, 0, source.size());
		} finally {
			if (source != null) {
				source.close();
			}
			if (destination != null) {
				destination.close();
			}
		}
	}

	public static void Delete(File dir) {
		if (dir.isDirectory()) {
			for (File temp : dir.listFiles()) {
				Delete(temp);
			}
		}
		dir.delete();
	}

	public static String readline(File file) {
        try {
            List<String> lines = readlines(file);
            for (String line : lines) {
                return line;
            }
        } catch (Exception ignore) {}
		return "";
	}

	public static List<String> readlines(File file) throws IOException {
		BufferedReader br = null;
		List<String> lines = new ArrayList<String>();
		try {
			br = new BufferedReader(new FileReader(file));
			String line;
			while ((line = br.readLine()) != null) {
				lines.add(line);
			}
		} finally {
			if(br != null){
				br.close();
			}
			return lines;
		}
	}

    public static void writeline(File file, String line) throws IOException {
        writelines(file, Arrays.asList(new String[]{line}));
    }

	public static void writelines(File file, List<String> lines) throws IOException {
		BufferedWriter bw = null;
		try {
			bw = new BufferedWriter(new FileWriter(file));
			for(String line : lines) {
				bw.append(line);
				bw.newLine();
			}
			bw.flush();
		} finally {
			if(bw != null){
				bw.close();
			}
		}
	}
}
