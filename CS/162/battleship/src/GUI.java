
import java.awt.Color;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;


public class GUI extends JFrame implements ActionListener {

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
	//The two JPanels to hold the player's buttons and the computer's buttons
	private JPanel player, opponent;
	
	//A two dimensional array of FireButtons to hold the buttons on the player board
	private FireButton[][] buttons;
	
	//The two Battleship Boards
	private BattleshipBoard pBoard, opBoard;
	
	//The two types of styles for the computer AI
	private SystematicPlayer systematic;
	private RandomPlayer random;
	private SmarterPlayer smarter;
	
	public GUI(String playerPath, String opPath, String mode) throws FileNotFoundException{
		//initialize both of the Battleship Boards by giving them a File corresponding to the text files as a parameter
		pBoard = new BattleshipBoard(new File(playerPath));
		opBoard = new BattleshipBoard(new File(opPath));
		
		//if the mode is r, create the random AI player, otherwise, create the systematic one
		if(mode.equals("r"))
			random = new RandomPlayer();
		else if(mode.equals("s"))
			systematic = new SystematicPlayer();
		else
			smarter = new SmarterPlayer();
		
		//set the properties of the JFrame
		setLayout(new GridLayout(1, 2));
		setSize(1000, 600);
		setTitle("Battleship Game");
		setDefaultCloseOperation(EXIT_ON_CLOSE);
		
		//initialize the player panel
		player = new JPanel();
		
		//make the player panel a 10x10 grid with a titled border
		player.setLayout(new GridLayout(10, 10));
		player.setBorder(BorderFactory.createTitledBorder("Player Battleship Board"));
		
		//initialize the opponent panel
		opponent = new JPanel();
		
		//make the opponent panel a 10x10 grid with a title border
		opponent.setLayout(new GridLayout(10, 10));
		opponent.setBorder(BorderFactory.createTitledBorder("Opponent Battleship Board"));
		
		//initialize the 2D array of FireButtons
		buttons = new FireButton[10][10];
		
		//nested for loop to initialize each FireButton in the array and add it to the player panel
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 10; j++){
				//initialize the button at j, i and disable it
				buttons[j][i] = new FireButton(j, i);
				buttons[j][i].setEnabled(false);
				
				//check if the place on the Battleship board is occupied by a ship
				if(pBoard.isOccupied(j, i)){
					//change the button's text to the number of the ship and make the button orange
					buttons[j][i].setBackground(Color.ORANGE);
					buttons[j][i].setText(pBoard.getCellContent(j, i) + "");
				}else{
					//make the button gray
					buttons[j][i].setBackground(Color.GRAY);
				}
				
				//add the FireButton to the player panel
				player.add(buttons[j][i]);
			}
		
		//nested for loop to create a new FireButton, set its ActionListener, and add it to the opponent panel
		for(int i = 0; i < 10; i++)
			for(int j = 0; j < 10; j++){
				//create a new FireButton and set its ActionListener
				FireButton b = new FireButton(j, i);
				b.addActionListener(this);
				
				//add the button to the opponent panel
				opponent.add(b);
			}
		
		//add the player and opponent panels to the JFrame
		add(player);
		add(opponent);
	}

	@Override
	public void actionPerformed(ActionEvent arg0) {
		//get the source of the ActionEvent and cast it to a FireButton
		FireButton button = (FireButton) arg0.getSource();
		
		//get the cell of the FireButton, so we can determine its location on the board
		Cell cell = button.getCell();
		
		//fire a shot at the cell on the Battleship Board that corresponds to the FireButtons cell
		if(opBoard.fireShot(cell.getColumn(), cell.getRow())){
			//if the shot was a hit, change the text to H and make the color of the FireButton red
			button.setBackground(Color.RED);
			button.setText("H");
		}else{
			//if the shot was a miss, change the text to M and make the color of the FireButton blue
			button.setBackground(Color.BLUE);
			button.setText("M");
		}
		
		//disable the button
		button.setEnabled(false);
		
		//check to see if the Player one
		if(opBoard.isGameOver())
			//if the game is over, end the game gracefully
			endGame(true);
		
		//determine which computer AI to use
		if(random == null && smarter == null)
			//if random is null, that means that we initialized systematic, so use it to fire
			systematic.fire(pBoard, buttons);
		else if(smarter == null)
			//if random isn't null, that means that we initialized random, so use it to fire
			random.fire(pBoard, buttons);
		else
			smarter.fire(pBoard, buttons);
		
		//check to see if the Opponent one
		if(pBoard.isGameOver())
			//if the game is over, end the game gracefully
			endGame(false);
	}
	
	private void endGame(boolean youWon){
		String message;
		
		//if youWon is true, set the Message to "YOU WIN!", otherwise, set it to "CPU WINS!"
		if(youWon)
			message = "YOU WIN!";
		else
			message = "CPU WINS!";
		
		//display a message box saying that the game is over and who won
		JOptionPane.showMessageDialog(null, message, "GAME OVER!", JOptionPane.INFORMATION_MESSAGE);
		
		//disable all the buttons on the opponents board (the player's board buttons are already disabled)
		for(int i = 0; i < opponent.getComponentCount(); i++)
			((FireButton) opponent.getComponent(i)).setEnabled(false);
	}
	
	public static void main(String[] args){
		try {
			GUI gui = new GUI(args[1], args[0], args[2]);
			gui.setVisible(true);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}
}
