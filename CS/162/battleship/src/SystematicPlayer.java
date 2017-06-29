import java.awt.Color;


public class SystematicPlayer implements Player {
	private int x;
	private int y;

	public SystematicPlayer(){
		//give x and y an initial value of 0
		x = y = 0;
	}

	@Override
	public void fire(BattleshipBoard board, FireButton[][] buttons) {
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

		//increment the column
		x ++;
		
		//if the column equals ten, set it to zero and increment the row
		if(x == 10){
			x = 0;
			y ++;
		}
	}

}
