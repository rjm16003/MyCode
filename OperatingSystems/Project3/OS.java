import java.io.*;


import java.util.Scanner;

import java.util.Queue;

import java.util.LinkedList;
/* Created by: Robert McNeil rjm160030
 * Class: CS4348.001 Operating Systems
 * File: OS.java
 * 
 * Summary: A code that implements 6 different 
 * scheduling schemes
 */


public class OS {
	
	public static abstract class Scheduler{
		public abstract void schedule(String storage[][]);
	}
	
	//print the formatted string
	public static void printString(String tempstring, String[][] storage, int finaltime){
		
		int numlines = storage.length;

		String[] tempsplit = tempstring.split(" ");			
		
		String[] pstring = new String[finaltime];
		for(int ix=0; ix<finaltime; ix++){
			pstring[ix] = " ";
		}
		
		for(int ix=1; ix<tempsplit.length; ix+=2){
			pstring[Integer.parseInt(tempsplit[ix])] = tempsplit[ix-1];
		}
		int currentline = 0;
		while(currentline < numlines){		
			for(int ix=0; ix<finaltime; ix++){			
				if(pstring[ix].equals(storage[currentline][0])) System.out.print(pstring[ix]);			
				else System.out.print(" ");		
			}		
			System.out.println();
			currentline++;
		}	

		
	}
	
	//create a copy of the 2d string array
	public static void copyString(String[][] storage, String[][] stemp){
		for(int ix=0; ix<storage.length; ix++){
			for(int iy=0; iy<storage[ix].length; iy++){
				stemp[ix] = storage[ix].clone();
			}
		}
	}
	
	//FIFO first in first out scheduler
	public static class FIFO extends Scheduler{
		
		public void schedule(String[][] storage2){
			int count = 0;
			System.out.print("\n");
			System.out.println("FIFO:");
			for(int ix=0; ix<storage2.length; ix++){
				for(int iy=0; iy<count; iy++){
					System.out.print(" ");
				}
				while(Integer.parseInt(storage2[ix][2]) > 0){
					System.out.print(storage2[ix][0]);
					storage2[ix][2] = Integer.toString(Integer.parseInt(storage2[ix][2]) - 1);
					count++;
				}
				while(ix+1<storage2.length && count < Integer.parseInt(storage2[ix+1][1])){
					count++;
				}
				System.out.println();
			}
		}
		
		
	}
	
	//RR Round-robin scheduler
	public static class RR extends Scheduler{		
		
		public void schedule(String[][] storage){
			Queue<String[]> q1 = new LinkedList<String[]>();			
			int numlines = storage.length;
			
			int finaltime = 0;
			finaltime = Integer.parseInt(storage[0][1]);

			for(int ix=0; ix<numlines; ix++){
				finaltime += Integer.parseInt(storage[ix][2]);
			}
			
//			int finaltime = Integer.parseInt(storage[numlines-1][2]) + Integer.parseInt(storage[numlines-1][1]);
			
			String tempstring = "";
			int time=0;
			//print the time as a single number separated by a space for each line
			//this "time" value is a placeholder for when to print the character
			int currentline = 0;
			time = Integer.parseInt(storage[currentline][1]);
			q1.add(storage[currentline]);
			currentline++;
			
			System.out.print("\n");
			
			System.out.println("RR, quantum = 1");
			while(time < finaltime){			
				if(currentline < numlines && Integer.parseInt(storage[currentline][1]) == time && !(q1.contains(storage[currentline][0]))){
					q1.add(storage[currentline]);
					if(numlines-currentline>0) currentline++;
				}
				if(!q1.isEmpty()){
					String[] temp = q1.remove();
					tempstring += temp[0] + " " + time + " ";
					int num = Integer.parseInt(temp[2]) - 1;
					if(num > 0){
						if((currentline) < numlines && Integer.parseInt(storage[currentline][1]) == (time+1)){
							q1.add(storage[currentline]);
							if(numlines-currentline>0) currentline++;
						}
						temp[2] = Integer.toString(num);
						q1.add(temp);
					}
				}	
				time++;
			}
			printString(tempstring, storage,finaltime);
		}		
	}
	
	//SPN shortest process next scheduler
	public static class SPN extends Scheduler{		
		
	public void schedule(String[][] storage){
			int numlines = storage.length;
			
			int finaltime = Integer.parseInt(storage[0][1]);

			for(int ix=0; ix<numlines; ix++){
				finaltime += Integer.parseInt(storage[ix][2]);
			}
						
			String tempstring = "";
			int time=0;
			int min = 0;
			while(time < finaltime){
				for(int index=0; index<numlines; index++){
					if(Integer.parseInt(storage[min][2]) == 0) min = index;
					if(Integer.parseInt(storage[index][1]) <= time && Integer.parseInt(storage[index][2]) <= Integer.parseInt(storage[min][2])){
						if(Integer.parseInt(storage[index][2]) > 0) min = index;
					}
				}
				while(Integer.parseInt(storage[min][2]) > 0){
					tempstring += storage[min][0] + " " + time + " ";
					storage[min][2] = Integer.toString(Integer.parseInt(storage[min][2]) - 1);
					time++;
				}
			}

			System.out.print("\n");
			System.out.println("SPN:");

			printString(tempstring, storage,finaltime);
	
		}		
	}
	
	//SRT shortest remaining time scheduler
	public static class SRT extends Scheduler{		
	
	public void schedule(String[][] storage){
		int numlines = storage.length;
		
		int finaltime = Integer.parseInt(storage[0][1]);

		for(int ix=0; ix<numlines; ix++){
			finaltime += Integer.parseInt(storage[ix][2]);
		}
					
		String tempstring = "";
		int time=0;
		int min = 0;
		while(time < finaltime){
			for(int index=0; index<numlines; index++){
				if(Integer.parseInt(storage[min][2]) == 0) min = index;
				if(Integer.parseInt(storage[index][1]) <= time && Integer.parseInt(storage[index][2]) <= Integer.parseInt(storage[min][2])){
					if(Integer.parseInt(storage[index][2]) > 0) min = index;
				}
			}
			while(Integer.parseInt(storage[min][2]) > 0){
				tempstring += storage[min][0] + " " + time + " ";
				storage[min][2] = Integer.toString(Integer.parseInt(storage[min][2]) - 1);
				time++;
			}
		}

		System.out.println("\n");

		System.out.println("SRT:");

		printString(tempstring, storage,finaltime);

	}		
}

	//HRRN highest response ratio next scheduler
	public static class HRRN extends Scheduler{		
	
	public void schedule(String[][] storage){
		int numlines = storage.length;
		
		int finaltime = Integer.parseInt(storage[0][1]);

		for(int ix=0; ix<numlines; ix++){
			finaltime += Integer.parseInt(storage[ix][2]);
		}
					
		String tempstring = "";
		int time=0;
		int max = 0;
		while(time < finaltime){
			double maxhrrn = 0;
			for(int index=0; index<numlines; index++){
				double hrrn = 0;
				double stime = Double.parseDouble(storage[index][2]);
				double wtime = time - Double.parseDouble(storage[index][1]);
				if(stime > 0){				
					hrrn = (wtime - stime)/stime;
					if(hrrn > maxhrrn){
						maxhrrn = hrrn;
						max = index;
					}
				}
				if(Integer.parseInt(storage[max][2]) == 0) max = index;
			}
			while(Integer.parseInt(storage[max][2]) > 0){
			tempstring += storage[max][0] + " " + time + " ";
			storage[max][2] = Integer.toString(Integer.parseInt(storage[max][2]) - 1);
			time++;
			}
		}

		System.out.print("\n");
		System.out.println("HRRN:");

		printString(tempstring, storage,finaltime);

	}		
}

	//feedback with 3 queues scheduler
	public static class Feedback extends Scheduler{		
		
		public void schedule(String[][] storage){
			Queue<String[]> q1 = new LinkedList<String[]>();
			Queue<String[]> q2 = new LinkedList<String[]>();
			Queue<String[]> q3 = new LinkedList<String[]>();
			
			int numlines = storage.length;
			
			int finaltime = 0;
			finaltime = Integer.parseInt(storage[0][1]);

			for(int ix=0; ix<numlines; ix++){
				finaltime += Integer.parseInt(storage[ix][2]);
			}
			
//			int finaltime = Integer.parseInt(storage[numlines-1][2]) + Integer.parseInt(storage[numlines-1][1]);
			
			String[] lines = new String[storage.length];
			String tempstring = "";
			int time=0;
			//print the time as a single number separated by a space for each line
			//this "time" value is a placeholder for when to print the character
			int currentline = 0;
			time = Integer.parseInt(storage[currentline][1]);
			q1.add(storage[currentline]);
			currentline++;
			while(time < finaltime){			
				if(currentline < numlines && Integer.parseInt(storage[currentline][1]) == time){
					if(!q1.contains(storage[currentline])){
					q1.add(storage[currentline]);
					if(numlines-currentline>0) currentline++;
				}}
				if(!q1.isEmpty()){
					String[] temp = q1.remove();
					tempstring += temp[0] + " " + time + " ";
					int num = Integer.parseInt(temp[2]) - 1;
					if(num > 0){
						temp[2] = Integer.toString(num);
						if((currentline) < numlines && Integer.parseInt(storage[currentline][1]) == (time+1)){
							q1.add(storage[currentline]);
							if(numlines-currentline>0) currentline++;
						}

						if(q1.isEmpty() && q2.isEmpty() && q3.isEmpty()) q1.add(temp);
						else q2.add(temp);
					}
				}	
				
				else if(!q2.isEmpty()){
					String[] temp = q2.remove();
					tempstring += temp[0] + " " + time + " ";
					int num = Integer.parseInt(temp[2]) - 1;
					if(num > 0){
						temp[2] = Integer.toString(num);
						if(q1.isEmpty() && q2.isEmpty() && q3.isEmpty()) q2.add(temp);
						else q3.add(temp);
					}
				}	
				else if(!q3.isEmpty()){
					String[] temp = q3.remove();
					tempstring += temp[0] + " " + time + " ";
					int num = Integer.parseInt(temp[2]) - 1;
					if(num > 0){
						temp[2] = Integer.toString(num);
						q3.add(temp);
					}
				}	

				time++;
			}
			
			System.out.print("\n");
			System.out.println("Feedback:");

			printString(tempstring, storage,finaltime);
			
		}
		
		
	}
	
	
	public static void main(String[] args){
		System.out.println("Option 1: FIFO");//basic menu
		System.out.println("Option 2: RR");
		System.out.println("Option 3: SPN");
		System.out.println("Option 4: SRT");
		System.out.println("Option 5: HRRN");
		System.out.println("Option 6: FEEDBACK");
		System.out.println("Option 7: ALL");
		System.out.print("Choose your option(i.e. 1): ");
		Scanner in = new Scanner(System.in);
		
		int choice;		
		choice = in.nextInt();
		
        String jobs = "jobs.txt";//get the file
        String line = null;
        int linenum = 0;

        try {
        	FileReader linefile = new FileReader(jobs);
            BufferedReader bufferedline = new BufferedReader(linefile);

            while((line = bufferedline.readLine()) != null) {
                linenum++;//get the number of lines in the file
            }   
            
            bufferedline.close(); 
            String[][] storage = new String[linenum][3];//create the string 2d array to store the data
            linenum = 0;

        	FileReader fileReader = new FileReader(jobs);
            BufferedReader bufferedReader = new BufferedReader(fileReader);

            while((line = bufferedReader.readLine()) != null) {
                storage[linenum] = line.split("\t");//store the data
                linenum++;
            }   
                        
            
            Scheduler test1 = null;//create the scheduler object
            
            switch(choice){
            case 1://fifo
            	test1 = new FIFO();
            	break;
            case 2://r
            	test1 = new RR();
            	break;
            case 3://spn
            	test1 = new SPN();
            	break;
            case 4://srt
            	test1 = new SRT();
            	break;
            case 5://hrrn
            	test1 = new HRRN();
            	break;
            case 6://feedback
            	test1 = new Feedback();
            	break;
            case 7://all
            	String[][] stemp = new String[storage.length][storage[0].length];
            	copyString(storage,stemp);
            	test1 = new FIFO();
                test1.schedule(stemp);
            	copyString(storage,stemp);
            	test1 = new RR();
                test1.schedule(stemp);
            	copyString(storage,stemp);
            	test1 = new SPN();
                test1.schedule(stemp);
            	copyString(storage,stemp);
            	test1 = new SRT();
                test1.schedule(stemp);
            	copyString(storage,stemp);
            	test1 = new HRRN();
                test1.schedule(stemp);
            	test1 = new Feedback();
            	break;
            }
            
            //display the schedule
            test1.schedule(storage);
            
            
            //close the file
            bufferedReader.close();         
        }
        catch(FileNotFoundException ex) {
            System.out.println(
                "Unable to open file '" + jobs + "'");                
        }
        catch(IOException ex) {
            System.out.println(
                "Error reading file '" + jobs + "'");                 
            }


	}

}
