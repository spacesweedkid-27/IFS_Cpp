package bin;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Scanner;

public class ParseFromCpp {
    // 2 = 4k
    private static final int RESOLUTION = 4;
    // Make it fit your screen
    private static final int HEIGHT = 1440 * RESOLUTION;
    // Make it fit your screen
    private static final int WIDTH = 2560 * RESOLUTION;

    // Image as int[][] with rgb values
    private static final int[][] image = new int[HEIGHT][WIDTH];

    public static void main(String[] args) throws IOException {
        // Readjust the crown
        if (args.length != 5) {
            System.out.println("Usage: java ParseFromCpp <path/to/plot.csv> <path/to/image.png> <X-Fit> <Y-Fit> <Scale>");
            //System.out.println("(X-Fit and Y-Fit transpose the image they should be notated as doubles)");
            System.exit(1);
        }

        double x_fit = Double.parseDouble(args[2]);
        double y_fit = Double.parseDouble(args[3]);
        double scale = Double.parseDouble(args[4]);

        // Initialise as tabula rasa
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                image[i][j] = Color.WHITE.getRGB();
            }
        }

        // Open the file
        FileInputStream fileInputStream = new FileInputStream(args[0]);
        // And make it useful
        Scanner scanner = new Scanner(fileInputStream);

        // Until we have more to go
        while (scanner.hasNext()){
            // Get the line
            String line = scanner.nextLine();
            // Convert it into left side and right side
            String[] split = line.split("\t");

            // Parse the "real" values
            double in_x = Double.parseDouble(split[0]) * scale;
            double in_y = Double.parseDouble(split[1]) * scale;

            // Now adjust it to the screen.
            // Note: 250 works really well for the output of the cpp test.
            int x = (int)(250 * RESOLUTION * in_x + HEIGHT/2 + x_fit * HEIGHT);
            int y = (int)(250 * RESOLUTION * in_y + WIDTH/2 + y_fit * WIDTH);

            // Implement some shading by only making everything darker
            // This makes the resulting image more beautiful as it isn't as sharp.
            try {
                image[x][y] = new Color(image[x][y]).darker().getRGB();
            } catch (ArrayIndexOutOfBoundsException ignore){
                // If we are out of bounds, we don't care for the color
            }

        }
        // Close the scanner and the inputstream.
        scanner.close();
        fileInputStream.close();

        // Now initialize a new BufferedImage which we will write to
        BufferedImage toImage = new BufferedImage(WIDTH, HEIGHT, 1);
        // Set every pixel to the according rgb value
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                toImage.setRGB(j, i, image[i][j]);
            }
        }
        // Open a new file that will become the image
        File outputFile = new File(args[1]);
        // Write the BufferedImage to this file.
        ImageIO.write(toImage, "PNG", outputFile);
    }
}
