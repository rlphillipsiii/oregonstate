import java.awt.Color;
import java.util.Random;


public class SmarterPlayer implements Player {
	private Cell lastHit;

	public void fire(BattleshipBoard board, FireButton[][] buttons){
		if(lastHit == null)
			fireAtRandom(board, buttons);
		else
			fireAroundLastHit(board, buttons);

	}

	private void fireAroundLastHit(BattleshipBoard board, FireButton[][] buttons){
		int x = lastHit.getColumn();
		int y = lastHit.getRow();
		
		if(checkBounds(x + 1, y) && !buttons[x + 1][y].getText().equals("H") &&
				!buttons[x + 1][y].getText().equals("M"))
			
			fireAtIndex(board, buttons, x + 1, y);
		
		else if(checkBounds(x, y + 1) && !buttons[x][y + 1].getText().equals("H") &&
				!buttons[x][y + 1].getText().equals("M"))
			
			fireAtIndex(board, buttons, x, y + 1);
		
		else if(checkBounds(x - 1, y) && !buttons[x - 1][y].getText().equals("H") &&
				!buttons[x - 1][y].getText().equals("M"))
			
			fireAtIndex(board, buttons, x - 1, y);
		
		else if(checkBounds(x, y - 1) && !buttons[x][y - 1].getText().equals("H") &&
				!buttons[x][y - 1].getText().equals("M"))
			
			fireAtIndex(board, buttons, x, y - 1);
		
		else{
			lastHit = null;
			fireAtRandom(board, buttons);
		}
	}

	private boolean checkBounds(int x, int y){
		if(x > 9 || y > 9 || x < 0 || y < 0)
			return false;

		return true;
	}

	private void fireAtRandom(BattleshipBoard board, FireButton[][] buttons){
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

		fireAtIndex(board, buttons, x, y);
	}

	private void fireAtIndex(BattleshipBoard board, FireButton[][] buttons, int x, int y){
		//fire a shot and update the board accordingly
		if(board.fireShot(x, y)){
			//if a ship was hit, change the FireButton's color to red and its text to "H"
			buttons[x][y].setBackground(Color.RED);
			buttons[x][y].setText("H");

			lastHit = new Cell(x, y);
		}else{
			//if a ship wasn't hit, change the FireButton's color to blue and its text to "M"
			buttons[x][y].setBackground(Color.BLUE);
			buttons[x][y].setText("M");
		}
	}
}
