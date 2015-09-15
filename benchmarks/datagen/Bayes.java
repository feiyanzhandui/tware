package datagen;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;

import datagen.Util;

public class Bayes {
    public static void main(String[] args) throws IOException {
        String file = args[0];
        int size = Integer.parseInt(args[1]);
        int attrs = Integer.parseInt(args[2]);
        long numData = size * Util.MB / Util.INT / (attrs + 1);
        Random r = new Random();
        BufferedWriter w = new BufferedWriter(new FileWriter(file));
        StringBuilder buff;
        for (long i = 0; i < numData; i++) {
            buff = new StringBuilder();
            for (int j = 0; j < attrs; j++) {
                buff.append(r.nextInt(10));
                buff.append(',');
            }
            buff.append(r.nextInt(10));
            buff.append('\n');
            w.write(buff.toString());
        }
        w.close();
        System.out.println("numData: " + numData);
    }
}
