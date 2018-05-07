package org.cocos2dx.cpp.ReadingBird;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class PhonemesSimilarity {
    private static Map<String, String> gNeighborPhonemes = new HashMap<>();
    private static String gDebugMessage = "";
    private static void init() {
        gNeighborPhonemes.put("aa", "aa ah er ao ae");
        gNeighborPhonemes.put("ae", "ae eh er ah");
        gNeighborPhonemes.put("ah", "ah ae er aa");
        gNeighborPhonemes.put("ao", "ao aa er uh");
        gNeighborPhonemes.put("aw", "aw aa uh ow");
        gNeighborPhonemes.put("ay", "ay aa iy oy ey ae");
        gNeighborPhonemes.put("b", "b p d");
        gNeighborPhonemes.put("ch", "ch sh jh t");
        gNeighborPhonemes.put("dh", "dh th z v");
        gNeighborPhonemes.put("d", "d t jh g b");
        gNeighborPhonemes.put("eh", "eh ih er ae");
        gNeighborPhonemes.put("er", "er eh ah ao");
        gNeighborPhonemes.put("ey", "ey eh iy ay");
        gNeighborPhonemes.put("f", "f hh th v");
        gNeighborPhonemes.put("g", "g k d");
        gNeighborPhonemes.put("hh", "hh th f p t k");
        gNeighborPhonemes.put("ih", "ih iy eh");
        gNeighborPhonemes.put("iy", "iy ih");
        gNeighborPhonemes.put("jh", "jh ch zh d");
        gNeighborPhonemes.put("k", "k g t hh");
        gNeighborPhonemes.put("l", "l r w");
        gNeighborPhonemes.put("m", "m n");
        gNeighborPhonemes.put("ng", "ng n");
        gNeighborPhonemes.put("n", "n m ng");
        gNeighborPhonemes.put("ow", "ow ao uh aw");
        gNeighborPhonemes.put("oy", "oy ao iy ay");
        gNeighborPhonemes.put("p", "p t b hh");
        gNeighborPhonemes.put("r", "r y l");
        gNeighborPhonemes.put("ss", "sh s z th");
        gNeighborPhonemes.put("sh", "sh s zh ch");
        gNeighborPhonemes.put("t", "t ch k d p hh");
        gNeighborPhonemes.put("th", "th s dh f hh");
        gNeighborPhonemes.put("uh", "uh ao uw uw");
        gNeighborPhonemes.put("uw", "uw uh uw");
        gNeighborPhonemes.put("v", "v f dh");
        gNeighborPhonemes.put("w", "w l y");
        gNeighborPhonemes.put("y", "y w r");
        gNeighborPhonemes.put("z", "z s dh z");
        gNeighborPhonemes.put("zh", "zh sh z jh");
    }

    public static double diceCoefficient(String s1, String s2) {
        ArrayList<String> s1List = getPhonemesList(s1);
        ArrayList<String> s2List = getPhonemesList(s2);
        ArrayList<String> intersection = new ArrayList<>();
        int s1Size = s1List.size();
        int s2Size = s2List.size();

        gDebugMessage = "\nf : " + s1List + "\n";
        gDebugMessage += "s : " + s2List + "\n";

        if (s1List.size() <= 2) {
            if (s2List.size() > 5) {
                gDebugMessage += "too many pronunciation";
                return 0;
            }
        } else {
            if (s1List.size() >= 10) {
                if ((int)(s1List.size() * 1.5f) <= s2List.size()) {
                    gDebugMessage += "too many pronunciation";
                    return 0;
                }

            } else {
                if (s1List.size() * 2 <= s2List.size()) {
                    gDebugMessage += "too many pronunciation";
                    return 0;
                }
            }

            if (s1List.size() / 2 >= s2List.size()) {
                gDebugMessage += "too little pronunciation";
                return 0;
            }
        }

        for (String phoneme : s1List) {
            int pos = findPhonemePosition(phoneme, s2List);
            if (pos != -1) {
                intersection.add(phoneme);
                s2List.remove(pos);
            }
        }

        gDebugMessage += "intersection : " + intersection;
        return (2 * intersection.size()) / (double)(s1Size + s2Size);
    }

    private static ArrayList<String> getPhonemesList(String phonemes) {
        String[] split = phonemes.split(" ");
        ArrayList<String> result = new ArrayList<>();

        if (split == null) {
            return result;
        }

        for (String s : split) {
            if (s.equals("SIL")) {
                continue;
            }

            if (s.startsWith("+")) {
                continue;
            }

            result.add(s);
        }
        return result;
    }

    private static int findPhonemePosition(String phoneme, ArrayList<String> data) {
        int result = -1;

        String phonemes = getNeighboringPhonemes(phoneme);
        String[] split = phonemes.split(" ");

        if (split == null || split.length == 0) {
            return result;
        }

        for (String s : split) {
            if (data.contains(s)) {
                result = data.indexOf(s);
                break;
            }
        }

        return result;
    }

    private static String getNeighboringPhonemes(String phoneme) {
        if (phoneme.equals("SIL")) {
            return null;
        }

        if (phoneme.startsWith("+")) {
            return null;
        }

        String result;
        if (gNeighborPhonemes.size() == 0) {
            init();
        }

        phoneme = phoneme.toLowerCase();
        result = gNeighborPhonemes.get(phoneme);
        if (result == null) {
            result = phoneme.toUpperCase();
        } else {
            result = result.toUpperCase();
        }

        return result;
    }

    public static String getDebugMessage() {
        return gDebugMessage;
    }
}
