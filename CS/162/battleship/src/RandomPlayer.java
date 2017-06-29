import java.awt.Color;
import java.util.Random;

public class RandomPlayer implements Player {

	public void fire(BattleshipBoard board, FireButton[][] buttons) {
		//create a new Random object
		Random rand = new Random();

		//create two random x and y coordinates to fire at
		int x = rand.nextInt(10);
		int y = rand.nextInt(10);

		//while the cell corresponding to x and y has been fired on, choose new coordinates
		while(buttons[x][y].getText().equals("H") ||
				buttons[x][y].getText().equals("M")){
			x = rand.nextInt(10);
			y = rand.nextInt(10);
		}

		//fire a shot and update the board accordingly
		if(board.fireShot(x, y)){
			//if a ship was hit, change the FireButton's color to red and its text to "H"
			buttons[x][y].setBackground(Color.RED);
			buttons[x][y].setText("H");
		}else{
			//if a ship wasn't hit, change the FireButton's color to blue and its text to "M"
			buttons[x][y].setBackground(Color.BLUE);
			buttons[x][y].setText("M");
		}
	}

}
