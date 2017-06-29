package com.thirdgen.chip8.gui;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.Semaphore;

import javax.swing.JFrame;

public class Chip8Launcher implements KeyListener {
	private static final int SCALER = 8;
	
	private Timer m_timer;
	private CPUClock m_clock;
	
	private byte m_keys[];
	
	public Chip8Launcher() 
	{
		m_keys = new byte[16];
	}
	
	public byte[] getKeys() 
	{
		return m_keys;
	}
	
	public void resetKeys() 
	{
		for (int i = 0; i < m_keys.length; i++) {
			m_keys[i] = 0x00;
		}
	}
	
	public void start() 
	{
		m_clock = new CPUClock();
		
		m_timer = new Timer();
		m_timer.scheduleAtFixedRate(m_clock, 0, 1);
	}
	
	public boolean isDirty() 
	{
		try {
			return m_clock.isDirty();
		} catch (InterruptedException e) { }
		
		return false;
	}
	
	public void clearDirty() 
	{
		try {
			m_clock.setDirty(false);
		} catch (InterruptedException e) { }
	}
	
	public boolean _60HzElapsed()
	{
		return m_clock._60HzElapsed();
	}
	
	@Override
	public void keyPressed(KeyEvent key) { }

	@Override
	public void keyReleased(KeyEvent key) 
	{ 
		resetKeys();
	}

	@Override
	public void keyTyped(KeyEvent key) 
	{ 		
		char map[] = {'x', '1', '2', '3', 'q', 'w', 'e', 'a', 's', 'd', 'z', 'c', '4', 'r', 'f', 'v'};
		
		char pressed = key.getKeyChar();
		for (int i = 0; i < map.length; i++) {
			m_keys[i] = (byte)((pressed == map[i]) ? 0x01 : 0x00); 
		}
	}
	
	public void printKeys() {
		for (int i = 0; i < m_keys.length; i++) {
			System.out.print(Integer.toHexString(m_keys[i] & 0xFF) + " ");
		}
		System.out.println();
	}
	
	private class Mutex extends Semaphore {
		private static final long serialVersionUID = 1L;

		public Mutex() { super(1); }
	}
	
	private class CPUClock extends TimerTask {
		private Mutex m_lock;
		private boolean m_dirty;
		private int m_count;
		
		public CPUClock() {
			m_lock = new Mutex();
			m_dirty = false;
			m_count = 0;
		}
		
		public boolean isDirty() throws InterruptedException {
			m_lock.acquire();
			boolean status = m_dirty;
			m_lock.release();
			return status;
		}
		
		public void setDirty(boolean dirty) throws InterruptedException {
			m_lock.acquire();
			m_dirty = dirty;
			m_lock.release();
		}
		
		public boolean _60HzElapsed()
		{
			boolean elapsed = (m_count >= 16);
			if (elapsed) {
				m_count = 16 - m_count;
			}
			return elapsed;
		}
		
		@Override
		public void run() 
		{
			m_count++;
			
			try {
				setDirty(true);
			} catch (InterruptedException e) { }
		}
	}
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		Chip8Launcher manager = new Chip8Launcher();
		Chip8Screen panel = new Chip8Screen("PONG");
		
		JFrame frame = new JFrame();
		frame.setTitle("Chip 8 Interpreter");
		
		frame.add(panel);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(64*SCALER, 32*SCALER);
		frame.setVisible(true);
		
		frame.addKeyListener(manager);
		
		manager.start();
		
		while (true) {
			if (manager._60HzElapsed()) {
				panel.decrementTimers();
			}
			
			if (manager.isDirty()) {
				panel.updateKeys(manager.getKeys());
	
				panel.nextCycle();
				if (panel.checkNeedsRefresh()) {
					panel.redraw();
				}
				
				manager.clearDirty();
			}
		}
	}

	

}
