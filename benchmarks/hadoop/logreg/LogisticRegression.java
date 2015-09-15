// Running
//     "bin/hadoop jar LinearRegressionMapReduce.jar LinearRegressionMapReduce

//                 <feature_size> <input_data_size>"
// Example
//     "bin/hadoop jar MultipleLinearRegressionMapReduce.jar MainClass
//                 input output 0.1 3 11 442


import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapreduce.*;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.util.ArrayList;
import java.net.URI;

import java.io.BufferedWriter;
import java.io.OutputStreamWriter;

public class LinearRegression {
    
    /*
     * Mapper start
     */
       
    public static class LinearRegressionMap extends Mapper<LongWritable, Text, LongWritable, FloatWritable>{
        
        FileInputStream fis = null;
        BufferedInputStream bis = null;
        private Float[] theta;
        private ArrayList<ArrayList<Float>> prediction_error = new ArrayList<ArrayList<Float>>();
        
        @Override
        public void setup(Context context) {
			String[] temp = context.getConfiguration().getStrings("theta");
			theta = new Float[temp.length];
			for (int i=0; i<temp.length; i++){
				theta[i] = Float.parseFloat(temp[i]);
			}
        }
        
        @Override
        protected void cleanup(Context context) throws IOException, InterruptedException{
            // aggregate results from the same map and then send to reducers
            Float[] temp = new Float[theta.length+1];
            for (int i=0; i < temp.length; i++) temp[i] = 0.0f;
            
            for (int i=0; i < prediction_error.size(); i++) // iterates on rows
                for (int j=0; j < prediction_error.get(i).size(); j++){ // iterates on columns
                    temp[j] += prediction_error.get(i).get(j);
                }
            
            for (int i=0; i < temp.length; i++)
                context.write(new LongWritable(i), new FloatWritable(temp[i]));
        }
        
        @Override
        public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
            String line = value.toString();
            String[] features = line.split(",");
            ArrayList<Float> values = new ArrayList<Float>();
            
            for (int i=0; i<features.length; i++)
                values.add(new Float(features[i]));
            
            prediction_error.add(compute_J(values));
        }
        
        private ArrayList<Float> compute_J(ArrayList<Float> values){
            
            // calculate prediction for each record in values
            ArrayList<Float> result = new ArrayList<Float>();
            float y = values.get(0);
            float h_theta = 0.0f;
            
            for (int i=0; i < values.size(); i++){
                if (i==0)
                    h_theta += theta[0];
                else 
                    h_theta += theta[i]*values.get(i);
            }
            
            
            float J_theta = (h_theta-y)*(h_theta-y);
            result.add(J_theta);
            
            for (int j=0; j < values.size(); j++){
                if (j==0)
                    result.add(h_theta-y);
                else
                    result.add((h_theta-y)*values.get(j)); 
            }
            return result;
        }
        
    }

    /*
     * Mapper end
     */
    
    
    /*
     * Reducer start
     */
    
    
    public static class LinearRegressionReduce extends Reducer<LongWritable, FloatWritable, LongWritable, FloatWritable> {
        private Float[] theta;
        Configuration conf;
        private Float alpha, prediction_error;
        private int input_data_size, iteration;
        
        @Override
        public void setup(Context context) {
            String[] temp = context.getConfiguration().getStrings("theta");
            theta = new Float[temp.length];
            for (int i=0; i<temp.length; i++)
                theta[i] = Float.parseFloat(temp[i]);
            alpha = context.getConfiguration().getFloat("alpha",0.1f);
            input_data_size = context.getConfiguration().getInt("input_data_size",1);
            iteration = context.getConfiguration().getInt("iteration",1);
        }
        
        @Override
        protected void cleanup(Context context) throws IOException{
            String uri = "/user/hduser/theta.txt";
            Path path = new Path( uri);
            
            
            try{
                FileSystem fs = FileSystem.get(URI.create( uri), context.getConfiguration());
                if (fs.exists(path)) fs.delete(path, true);
                BufferedWriter br=new BufferedWriter(new OutputStreamWriter(fs.create(path,true)));
                for (int i=0; i < theta.length; i++)
                    br.write(theta[i].toString()+",");
                br.write("\n");
                br.close();
            }catch(Exception e){
                System.out.println("File not found");
            }
            
            uri = "/user/hduser/LinearReg/theta-"+iteration+".txt";
            path = new Path(uri);
            
            try{
                FileSystem fs = FileSystem.get(context.getConfiguration());
                if (iteration == 0)
                    fs.delete(new Path("/user/hduser/LinearReg"), true);
                OutputStreamWriter osw = new OutputStreamWriter(fs.create(path,true));
                BufferedWriter br=new BufferedWriter(osw);
                br.write(prediction_error + ", ");
                for (int i=0; i < theta.length; i++)
                    br.write(theta[i].toString()+", ");
                br.write("\n");
                br.close();
            }catch(Exception e){
                System.out.println("File not found");
            }
        }
        
        @Override
        public void reduce(LongWritable key, Iterable<FloatWritable> values, Context context) throws IOException, InterruptedException {
            
            if ((int) key.get() == 0 ){
                Float cost = 0.0f;
                for (FloatWritable val : values)
                    cost += val.get();
                prediction_error = cost;
                context.write(key, new FloatWritable(cost));
            } else { // extracts theta
                Float cost = 0.0f;
                for (FloatWritable val : values)
                    cost += val.get();   
                
                System.out.println("cost for key: " + cost);
                System.out.println("cost  "+ cost*alpha/input_data_size);
                
                int key_index = (int) key.get()-1;
                System.out.println("key_index: "+key_index);
                
                theta[key_index] -= cost*alpha/input_data_size;
                context.write(key, new FloatWritable(cost));
            }
            
        }
        
        
    }
    
    /*
     *  Reducer end
     */
    
    /*
     * Main start
     */
	  
	  public static void main(String[] args) throws Exception {
		  String[] theta;
		  int iteration = 0, num_of_iteration = 1;
		  int feature_size =0, input_data_size=0;
		  FileSystem fs;
		  Float alpha = 0.1f;
		  
		  do {
			  Configuration conf = new Configuration();
			  fs = FileSystem.get(conf);
	           
			  Job job = new Job(conf, "LinearRegressionMapReduce");
			  job.setJarByClass(MultipleLinearRegressionMapReduce.class);
			  
			  conf = job.getConfiguration();
			  
			  job.setOutputKeyClass(LongWritable.class);
			  job.setOutputValueClass(FloatWritable.class); 
		           
			  job.setMapperClass(MultipleLinearRegressionMap.class);
			  job.setReducerClass(MultipleLinearRegressionReduce.class);
		           
			  job.setInputFormatClass(TextInputFormat.class);
			  job.setOutputFormatClass(TextOutputFormat.class);
			  
			  job.setNumReduceTasks(1);
		  
			  FileInputFormat.addInputPath(job, new Path(args[0]));
			  Path out = new Path(args[1]);
			  if (fs.exists(out)) fs.delete(out, true);
			  
			  FileOutputFormat.setOutputPath(job, out);
			  alpha = Float.parseFloat(args[2]);
			  num_of_iteration = Integer.parseInt(args[3]);
			  feature_size = Integer.parseInt(args[4]);
			  input_data_size = Integer.parseInt(args[5]);
			  conf.setFloat("alpha", alpha);
			  conf.setInt("feature_size", feature_size);
			  conf.setInt("input_data_size", input_data_size);
			  conf.setInt("iteration", iteration);
			  
			  theta = new String[feature_size];
		       
			  if (iteration == 0){
				  for (int i=0; i< theta.length; i++)
					  theta[i]="0.0";
				  conf.setStrings("theta", theta);
			  }else{
				  try{
					  String uri = "/user/hduser/theta.txt";
					  fs = FileSystem.get(conf);
					  BufferedReader br=new BufferedReader(new InputStreamReader(fs.open(new Path(uri))));
					  theta = br.readLine().split(",");
				  }catch (Exception e){
					  
				  } 
				  conf.setStrings("theta", theta);
			  }
            
			  try {
				  job.waitForCompletion(true);
				  iteration++;
			  } catch (IOException e) {
				  e.printStackTrace();
			  }
		 } while (iteration < num_of_iteration);
		    
    
	  }
    /*
     * Main end
     */
    
   }
