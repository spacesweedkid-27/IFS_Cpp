package lib;

public class FunctionSet {
    private double[][] values;
    private double[] probs;

    public double[] calculate(double[] in){
        double[] temp = new double[]{0, 0};



        return temp;
    }

    private static int pick_index(double[] probs){
        double[] sums = new double[probs.length];
        for (int i = 0; i < probs.length; i++) {
            double temp = 0;
            for (int j = 0; j < i; j++) {
                temp += probs[j];
            }
        }
        return 0;
    }
}
