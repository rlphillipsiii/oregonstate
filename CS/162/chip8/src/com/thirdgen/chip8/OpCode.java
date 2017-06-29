package com.thirdgen.chip8;

import java.util.Random;

public class OpCode {
	private static final int BYTE = 8;
		
	private static boolean g_debug = false;
	
	public static void enableDebugMessages(boolean debug)
	{
		g_debug = debug;
	}
	
	public static void clearPixels(byte pixels[]) 
	{
		if (g_debug) {
			System.out.println("clear screen");
		}
		
		for (int i = 0; i < pixels.length; i++) {
			pixels[i] = 0x00;
		}
	}
	
	public static void ret(Register pc, Stack stack) 
	{
		if (g_debug) {
			System.out.println("return");
		}
		
		pc.set(stack.pop());
	}
	
	public static void jump(Register pc, int opcode) 
	{
		if (g_debug) {
			System.out.println("jump");
		}
		
		pc.set((opcode & 0x0FFF));
	}
	
	public static void call(Register pc, Stack stack, int opcode) 
	{
		if (g_debug) {
			System.out.println("call");
		}
		
		stack.push(pc.getValue());
		
		pc.set((opcode & 0x0FFF));
	}
	
	public static void skipIfEqual(Register pc, Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if equal");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int val = (opcode & 0x00FF);
		
		if (V[reg].compare(val) == Register.EQUALS) {
			pc.increment(2);
		}
	}
	
	public static void skipIfNotEqual(Register pc, Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if not equal");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int val = (opcode & 0x00FF);
		
		if (V[reg].compare(val) != Register.EQUALS) {
			pc.increment(2);
		}
	}
	
	public static void skipIfRegEqual(Register pc, Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if reg equal");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		if (V[r1].compare(V[r2]) == Register.EQUALS) {
			pc.increment(2);
		}
	}
	
	public static void skipIfRegNotEqual(Register pc, Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if reg not equal");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		if (V[r1].compare(V[r2]) != Register.EQUALS) {
			pc.increment(2);
		}
	}
	
	public static void setRegImmediate(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("set register immediate");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int val = (opcode & 0x00FF);
		
		V[reg].set(val);
	}
	
	public static void addRegImmediate(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("add register immediate");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int val = (opcode & 0x00FF);
		
		V[reg].increment(val);
	}
	
	public static void setReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("set register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[r1].set(V[r2]);
	}
	
	public static void orReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("or register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[r1].orE(V[r2]);
	}
	
	public static void andReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("and register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[r1].andE(V[r2]);
	}
	
	public static void xorReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("xor register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[r1].xorE(V[r2]);
	}
	
	public static void addReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("add register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		long sum = V[r1].getValue() + V[r2].getValue();
		V[0xF].set((sum > 255) ? 0x01 : 0x00);
		
		V[r1].increment(V[r2]);
	}
	
	public static void subReg(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("subtract register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[0xF].set((V[r1].compare(V[r2]) == Register.LESS_THAN) ? 0x00 : 0x01);
		
		V[r1].decrement(V[r2]);
	}
	
	public static void addNegative(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("add negative register");
		}
		
		int r1 = (opcode & 0x0F00) >> 8;
		int r2 = (opcode & 0x00F0) >> 4;
		
		V[0xF].set((V[r2].compare(V[r1]) == Register.LESS_THAN) ? 0x00 : 0x01);
		
		V[r1].set(V[r2].sub(V[r1]));
	}
	
	public static void rotateRight(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("rotate right");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		V[0xF].set(V[reg].and(0x01));
		
		V[reg].shiftRight(0x01);;
	}
	
	public static void rotateLeft(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("rotate left");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		V[0xF].set(((V[reg].getValue() & 0xFF) >> 7));
		
		V[reg].shiftLeft(0x01);
	}
	
	public static void setI(Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("set i register");
		}
		
		i.set(opcode & 0x0FFF);
	}
	
	public static void jumpRelative(Register pc, Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("jump relative");
		}
		
		long jmp = (opcode & 0x0FFF);
		
		pc.set(jmp + (V[0].getValue() & 0xFF));
	}
	
	public static void setRegRandom(Register V[], int opcode) 
	{
		if (g_debug) {
			System.out.println("set register random");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int val = (opcode & 0x00FF);
		
		int r = new Random().nextInt()%255;
		
		V[reg].set((r & val));
	}
	
	public static void updatePixels(Register memory[], byte pixels[], Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("update pixels");
		}
		
		int x = (int)V[(opcode & 0x0F00) >> 8].getValue();
		int y = (int)V[(opcode & 0x00F0) >> 4].getValue();
		int height = (opcode & 0x000F);
		
		V[0xF].set(0x00);
		for (int row = 0; row < height; row++) {
			int address = (int)(i.getValue() + row);
			
			long pixel = memory[address].getValue();
			
			for (int col = 0; col < BYTE; col++) {
				if ((pixel & (0x80 >> col)) != 0) {
					int index = (x + col) + ((y + row)*64);
					
					if (index >= pixels.length) {
						index -= pixels.length;
					} else if (index < 0) {
						index += pixels.length;
					}
					
					if (pixels[index] == 0x01) {
						V[0xF].set(0x01);
					}
					
					pixels[index] ^= 0x01;
				}
			}
		}
	}
	
	public static void skipIfButtonPressed(byte keys[], Register V[], Register pc, int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if button pressed");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		if (keys[(int)V[reg].getValue()] != 0x00) {
			pc.increment(2);
		}
	}
	
	public static void skipIfButtonNotPressed(byte keys[], Register V[], Register pc, int opcode) 
	{
		if (g_debug) {
			System.out.println("skip if button not pressed");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		if (keys[(int)V[reg].getValue()] == 0x00) {
			pc.increment(2);
		}
	}
	
	public static void setRegToDelayTimer(Register V[], Register delay, int opcode) 
	{	
		if (g_debug) {
			System.out.println("set reg to delay timer");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		V[reg].set(delay);
	}
	
	public static void setDelayToReg(Register V[], Register delay, int opcode) 
	{	
		if (g_debug) {
			System.out.println("set delay timer to register");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		delay.set(V[reg]);
	}
	
	public static void setSoundToReg(Register V[], Register sound, int opcode) 
	{
		if (g_debug) {
			System.out.println("set sound timer to register");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		sound.set(V[reg]);
	}
	
	public static void addRegToI(Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("add register to i register");
		}
		
		int reg  = (opcode & 0x0F00) >> 8;
		long val = i.add(V[reg]);
		
		V[0xF].set((val > 0xFFF) ? 0x01 : 0x00);
		
		i.set(val);
	}
	
	public static void storeBCD(Register memory[], Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("store bcd");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		int num = (int) V[reg].getValue();
		
		int digits[] = new int[3];
		digits[0] = num/100;
		digits[1] = (num%100)/10;
		digits[2] = num%10;
		
		for (int ii = 0; ii < digits.length; ii++) {
			memory[(int)i.add(ii)].set(digits[ii]);
		}
	}
	
	public static void storeRegisters(Register memory[], Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("store registers");
		}
		
		int limit = (opcode & 0x0F00) >> 8;
		
		for (int ii = 0; ii <= limit; ii++) {
			memory[(int)i.add(ii)].set(V[ii]);
		}
		
		i.increment(limit + 1);
	}
	
	public static void fillRegisters(Register memory[], Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("fill registers");
		}
		
		int limit = (opcode & 0x0F00) >> 8;
		
		for (int ii = 0; ii <= limit; ii++) {
			V[ii].set(memory[(int)i.add(ii)]);
		}
		
		i.increment(limit + 1);
	}
	
	public static void waitForKeyPress(Register V[], byte[] keys, Register pc, int opcode) 
	{
		if (g_debug) {
			System.out.println("wait for key press");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		boolean pressed = false;
		
		for (byte i = 0; i < keys.length; i++) {
			if (keys[i] != 0x00) {
				V[reg].set(i);
				pressed = true;
				
				break;
			}
		}
		
		if (!pressed) {
			pc.decrement(2);
		}
	}
	
	public static void setIToSprite(Register V[], Register i, int opcode) 
	{
		if (g_debug) {
			System.out.println("set i to sprite");
		}
		
		int reg = (opcode & 0x0F00) >> 8;
		
		i.set(V[reg].getValue() * 5);
	}
}
