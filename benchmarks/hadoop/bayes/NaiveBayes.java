// Example
//     "bin/hadoop jar NaiveBayes.jar
//      NaiveBayesClassifierMapReduce
//      input output #ofclasses #of features"
//

import java.io.IOException;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class NaiveBayes {

	public static void main(String[] args) throws IOException, InterruptedException, ClassNotFoundException {
		
		int number_of_classes = 1;
		int number_of_features = 1;
		Configuration conf = new Configuration();
		FileSystem fs = FileSystem.get(conf);
	           
		Job job = new Job(conf, "NaiveBayesClassifier");
		job.setJarByClass(NaiveBayesClassifierMapReduce_Continuous_Features.class);
			  
   	    conf = job.getConfiguration(); // This line is mandatory. 

		job.setOutputKeyClass(LongWritable.class);
		job.setOutputValueClass(FloatArrayWritable.class); 
		
		job.setMapOutputKeyClass(LongWritable.class);
		job.setMapOutputValueClass(MapArrayWritable.class); 
		           
		job.setMapperClass(NaiveBayesClassifierMap_Continuous_Features.class);
		job.setReducerClass(NaiveBayesClassifierReduce_Continuous_Features.class);
		           
		job.setInputFormatClass(TextInputFormat.class);
		job.setOutputFormatClass(TextOutputFormat.class);
		
		job.setNumReduceTasks(1);
		  
		FileInputFormat.addInputPath(job, new Path(args[0]));
		Path out = new Path(args[1]);
		if (fs.exists(out)) fs.delete(out, true);
		FileOutputFormat.setOutputPath(job, out);
		number_of_classes = Integer.parseInt(args[2]);
		number_of_features = Integer.parseInt(args[3]);
		conf.setInt("number_of_classes", number_of_classes);
		conf.setInt("number_of_features", number_of_features);
		
	    try {
	    	job.waitForCompletion(true);
			
	    } catch (IOException e) {
	    	e.printStackTrace();
	    }
	}

}
