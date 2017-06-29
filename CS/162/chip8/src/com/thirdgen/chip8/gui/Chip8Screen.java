package com.thirdgen.chip8.gui;

import java.awt.Color;
import java.awt.Graphics;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;

import javax.swing.JPanel;

import com.thirdgen.chip8.Chip8;

public class Chip8Screen extends JPanel {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private static final int SCALER = 7;
	
	private Chip8 m_processor;

	public Chip8Screen(String filepath) 
	{
		this.setBackground(Color.BLACK);
				
		m_processor = new Chip8();
		
		try {
			byte[] rom = Files.readAllBytes(Paths.get(filepath));
			for (int i = 0; i < rom.length; i++) {
				m_processor.setMemory(0x200 + i, rom[i]);
			}
		} catch (IOException e) {
			e.printStackTrace();
			System.exit(1);
		}		
	}
	
	public void updateKeys(byte keys[]) 
	{
		m_processor.updateKeys(keys);
	}
	
	public boolean checkNeedsRefresh() 
	{
		return m_processor.doesScreenNeedRedraw();
	}
	
	public void decrementTimers()
	{
		m_processor.decrementDelay();
		m_processor.decrementSound();
	}
	
	public void redraw() 
	{
		this.repaint(0);
		
		m_processor.clearRedrawFlag();
	}
	
	public void nextCycle() 
	{
		try {
			m_processor.doEmulateCycle();
		} catch (Exception e) {
			e.printStackTrace();
			System.exit(1);
		}
	}
	
	@Override
	public void paintComponent(Graphics g) 
	{
		super.paintComponent(g);
		
		g.setColor(Color.WHITE);
		
		byte gfx[] = m_processor.getPixelArray();
		for (int i = 0; i < gfx.length; i++) {
			if (gfx[i] != 0) {
				int col = i%64;
				int row = i/64;
			
				g.fillRect(col*SCALER, row*SCALER, 1*SCALER, 1*SCALER);
			}
		}
	}
}
