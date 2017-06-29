/**
 * @author
 * 
 * Unit tests for automated verification of the Xiangqi chess game
 */
import java.awt.Color;
import junit.framework.TestCase;

import org.junit.Before;
import org.junit.Test;

public class XiangqiTester extends TestCase {
	/**
	 * Game that will be tested against
	 */
	private Xiangqi m_game;
	
	/**
	 * Setup routine to be done before each test
	 */
	@Before
	public void setUp() throws Exception 
	{
		m_game = new Xiangqi();
	}

	/**
	 * Test the testLegalPieceToPlay method of the Xiangqi game
	 */
	@Test
	public void testLegalPieceToPlay() 
	{
		ChessPiece p = new PawnPiece(ChessGame.Side.NORTH, m_game.getBoard());
		if (m_game.legalPieceToPlay(p)) {
			fail("Piece should not be legal");
		}
		
		p = new PawnPiece(ChessGame.Side.SOUTH, m_game.getBoard());
		if (!m_game.legalPieceToPlay(p)) {
			fail("Piece should be legal");
		}
	}

	/**
	 * Test the makeMove method of the Xiangqi game
	 */
	@Test
	public void testMakeMove() 
	{
		ChessBoard b = m_game.getBoard();
		
		/* King Piece of South player */
		ChessPiece p = b.getPiece(9, 4);
		
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Single move forward for king is legal");
		}
		
		m_game.makeMove(p, 7, 4);
		if (m_game.makeMove(p, 6, 4)) {
			fail("Piece can't run it's teammate over");
		}
	}
	
	/**
	 * Test the addPiece method of the chess board
	 */
	@Test
	public void testAddPiece()
	{
		ChessPiece p = new PawnPiece(ChessGame.Side.NORTH, m_game.getBoard());
		
		ChessBoard b = m_game.getBoard();
		b.addPiece(p, 7, 2);
		
		if (b.getPiece(7, 2) == null) {
			fail("Failed to place piece at coordinates");
		}
	}
	
	/**
	 * Test the removePiece method of the chess board
	 */
	@Test
	public void testRemovePiece()
	{
		ChessBoard b = m_game.getBoard();
		
		ChessPiece p = b.removePiece(7, 1);
		if (p == null) {
			fail("Removed piece should not be null");
		}
		
		if (b.getPiece(7, 1) != null) {
			fail("Square should have null piece on it");
		}
	}
	
	/**
	 * Test the hasPiece method of the chess board
	 */
	@Test
	public void testHasPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		if (b.hasPiece(7, 2)) {
			fail("There should be no piece at this location");
		}
		
		if (!b.hasPiece(7, 1)) {
			fail("There should be a piece at this location");
		}
	}
	
	/**
	 * Test the getPiece method of the chess board
	 */
	@Test
	public void testGetPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		if (b.getPiece(7, 1) == null) {
			fail("Square should not have null piece on it");
		}
		
		if (b.getPiece(7, 2) != null) {
			fail("Square should have null piece on it");
		}
	}
	
	/**
	 * Test methods that are common to all chess pieces
	 */
	@Test
	public void testCommonChessPieceFunctions()
	{
		ChessBoard b = m_game.getBoard();
		
		/* King Piece of South player */
		ChessPiece p = b.getPiece(9, 4);
		
		if (!p.getLabel().equals("K")) {
			fail("getLabel() failed");
		}
			
		if (p.getColor() != Color.WHITE) {
			fail("getColor() failed");
		}
		
		if (p.getRow() != 9) {
			fail("getRow() failed");
		}
		
		if (p.getColumn() != 4) {
			fail("getColumn() failed");
		}
		
		if (p.getSide() != ChessGame.Side.SOUTH) {
			fail("getSide() failed");
		}
		
		if (p.getIcon() != null) {
			fail("getIcon() failed");
		}
	}

	/**
	 * Test the move logic for a king
	 */
	@Test
	public void testKingPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* King Piece of South player */
		ChessPiece p = b.getPiece(9, 4);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Single move vertical for king is legal");
		}
		
		if (!m_game.makeMove(p, 8, 3)) {
			fail("Single move horizontal for king is legal");
		}
		
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Single move horizontal for king is legal");
		}
		
		if (!m_game.makeMove(p, 9, 4)) {
			fail("Single move vertical for king is legal");
		}
		
		if (m_game.makeMove(p, 7, 4)) {
			fail("Double move vertical for king is illegal");
		}
		
		if (m_game.makeMove(p, 9, 6)) {
			fail("Double move horizontal for king is illegal");
		}
		
		if (m_game.makeMove(p, 8, 5)) {
			fail("Diagonal for king is illegal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 8, 4);
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Take move for king is legal");
		}
	}
	
	/**
	 * Test the move logic for a cannon
	 */
	@Test
	public void testCannonPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Cannon Piece of South player */
		ChessPiece p = b.getPiece(7, 7);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 0, 7)) {
			fail("Move vertical for cannon is legal");
		}
		
		if (!m_game.makeMove(p, 0, 0)) {
			fail("Move horizontal for cannon is legal");
		}
		
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Move horizontal for cannon is legal");
		}
		
		if (!m_game.makeMove(p, 7, 7)) {
			fail("Move vertical for cannon is legal");
		}
		
		if (m_game.makeMove(p, 2, 2)) {
			fail("Diagonal for cannon is illegal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 5, 7);
		if (m_game.makeMove(p, 2, 7)) {
			fail("Cannon can't jump on non takes");
		}
		
		if (m_game.makeMove(p, 5, 7)) {
			fail("Cannon can't take pieces without jump");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 2, 7);
		if (!m_game.makeMove(p, 2, 7)) {
			fail("Cannon can take pieces with jump");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.SOUTH, b), 3, 7);		
		if (m_game.makeMove(p, 3, 7)) {
			fail("Cannon can't jump multiple pieces");
		}
	}
	
	/**
	 * Test the move logic for a rook
	 */
	@Test
	public void testRookPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Rook Piece of South player */
		ChessPiece p = b.getPiece(9, 0);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 0, 0)) {
			fail("Move vertical for rook is legal");
		}
		
		if (!m_game.makeMove(p, 0, 8)) {
			fail("Move horizontal for rook is legal");
		}
		
		if (!m_game.makeMove(p, 9, 8)) {
			fail("Move horizontal for rook is legal");
		}
		
		if (!m_game.makeMove(p, 9, 0)) {
			fail("Move vertical for rook is legal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.SOUTH, b), 9, 7);		
		if (m_game.makeMove(p, 9, 8)) {
			fail("Rook can't jump pieces");
		}
		
		if (!m_game.makeMove(p, 9, 5)) {
			fail("Rook can take opponents on vertical/horizontal move");
		}
	}
	
	/**
	 * Test the move logic for a elephant
	 */
	@Test
	public void testElephantPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Elphant Piece of South player */
		ChessPiece p = b.getPiece(9, 2);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 9, 2)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 7, 4)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 9, 2)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (m_game.makeMove(p, 8, 1)) {
			fail("Move single diagonal for elephant is illegal");
		}
		
		if (m_game.makeMove(p, 7, 1)) {
			fail("Move invalid diagonal for elephant is illegal");
		}
		
		if (m_game.makeMove(p, 8, 2)) {
			fail("Move vertical for elephant is illegal");
		}
		
		if (m_game.makeMove(p, 9, 1)) {
			fail("Move horizontal for elephant is illegal");
		}
		
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Move for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 5, 2)) {
			fail("Move for elephant is legal");
		}
		
		if (m_game.makeMove(p, 3, 0)) {
			fail("Move past middle for elephant is illegal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.SOUTH, b), 6, 3);
		if (m_game.makeMove(p, 7, 4)) {
			fail("Move over pieces for elephant is illegal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 7, 0);
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Legal take move for elephant");
		}
	}
	
	/**
	 * Test the move logic for a pawn
	 */
	@Test
	public void testPawnPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Pawn Piece of South player */
		ChessPiece p = b.getPiece(6, 0);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 5, 0)) {
			fail("Legal move for pawn");
		}
		
		if (!m_game.makeMove(p, 4, 0)) {
			fail("Legal move for pawn");
		}
		
		if (!m_game.makeMove(p, 4, 1)) {
			fail("Legal move for pawn");
		}
		
		if (!m_game.makeMove(p, 4, 0)) {
			fail("Legal move for pawn");
		}
		
		if (m_game.makeMove(p, 3, 1)) {
			fail("Invalid diagonal move for pawn");
		}
		
		if (m_game.makeMove(p, 4, 0)) {
			fail("Invalid backwards move for pawn");
		}
		
		if (m_game.makeMove(p, 2, 0)) {
			fail("Invalid forward move for pawn");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 3, 0);
		if (!m_game.makeMove(p, 3, 0)) {
			fail("Legal take move for elephant");
		}
	}
	
	/**
	 * Test the move logic for a guard
	 */
	@Test
	public void testGuardPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Guard Piece of South player */
		ChessPiece p = b.getPiece(9, 3);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 7, 3)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (!m_game.makeMove(p, 9, 3)) {
			fail("Move diagonal for elephant is legal");
		}
		
		if (m_game.makeMove(p, 8, 2)) {
			fail("Move diagonal is out of bounds");
		}
		
		if (m_game.makeMove(p, 7, 2)) {
			fail("Move multiple diagonal is illegal");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 8, 4);
		if (!m_game.makeMove(p, 8, 4)) {
			fail("Legal take move for elephant");
		}
	}
	
	/**
	 * Test the move logic for a knight
	 */
	@Test
	public void testKnightPiece()
	{
		ChessBoard b = m_game.getBoard();
		
		/* Knight Piece of South player */
		ChessPiece p = b.getPiece(9, 1);
		b.removeAllPieces();
		
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Legal L move for knight");
		}
		
		if (!m_game.makeMove(p, 6, 2)) {
			fail("Legal L move for knight");
		}
		
		if (m_game.makeMove(p, 6, 3)) {
			fail("Illegal horizontal move for knight");
		}
		
		if (m_game.makeMove(p, 7, 3)) {
			fail("Illegal diagonal move for knight");
		}
		
		if (m_game.makeMove(p, 7, 2)) {
			fail("Illegal vertical move for knight");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 6, 3);
		if (m_game.makeMove(p, 7, 4)) {
			fail("Knight can't jump pieces");
		}
		
		b.addPiece(new PawnPiece(ChessGame.Side.NORTH, b), 7, 0);
		if (!m_game.makeMove(p, 7, 0)) {
			fail("Legal take move for knight");
		}
	}
}
