import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.MapWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class NaiveBayesMap extends Mapper<LongWritable, Text, LongWritable, MapArrayWritable>{
	
	private Float[] features;
	private int class_id;
	private int number_of_classes, number_of_features; 
	private Map<Integer, ArrayList<Float[]>> features_probabilities = new HashMap<Integer,ArrayList<Float[]>>();
    
    private int[] num_of_members_in_each_class;
    private int[][] features_probabilities_arr;

	@Override
	public void setup(Context context) {
		number_of_classes = context.getConfiguration().getInt("number_of_classes", 1);
		number_of_features = context.getConfiguration().getInt("number_of_features", 1);
		num_of_members_in_each_class = new int[number_of_classes];
        
        features_probabilities_arr = new int[number_of_classes][number_of_features];
	}
	
	@Override
	protected void cleanup(Context context) throws IOException, InterruptedException{
        Float[] sigma_x = new Float[number_of_features];
        Float[] mu_x_local = new Float[number_of_features];
        Float[] num_x_local = new Float[number_of_features];
        MapWritable[] map_output = new MapWritable[number_of_features];
        
		for (int class_id=0; class_id < number_of_classes; class_id++){
			for (int i=0; i<number_of_features;i++){
	        	map_output[i] = new MapWritable();
	        	sigma_x[i] = 0.0f;
	            mu_x_local[i] = 0.0f;
	            num_x_local[i] = 0.0f;
	        }
            
            for(int feature_id = 0; feature_id < number_of_features; feature_id++){
                sigma_x[feature_id] = features_probabilities_arr[class_id][feature_id] * 1.0f;
            }
            
			for (int feature_id = 0; feature_id < number_of_features; feature_id++){
				num_x_local[feature_id] = (float)num_of_members_in_each_class[class_id];
				if (num_x_local[feature_id]==0)
					mu_x_local[feature_id] = 0.0f;
				else
					mu_x_local[feature_id] = sigma_x[feature_id]/num_x_local[feature_id];
			}
			         
            
			for (int feature_id_in_a_member_id=0; feature_id_in_a_member_id < number_of_features; feature_id_in_a_member_id++){
				map_output[feature_id_in_a_member_id].put(new Text("sigma_x"), new FloatWritable(sigma_x[feature_id_in_a_member_id]));
				map_output[feature_id_in_a_member_id].put(new Text("mu_x_local"), new FloatWritable(mu_x_local[feature_id_in_a_member_id]));
				map_output[feature_id_in_a_member_id].put(new Text("num_x_local"), new FloatWritable(num_x_local[feature_id_in_a_member_id]));
			}
			
			context.write(new LongWritable(class_id), new MapArrayWritable(map_output));
		}
        
        
	}

    @Override
	public void map(LongWritable key, Text value, Context context) throws IOException, InterruptedException {
		String line = value.toString();
		String[] features_str = line.split(",");
		class_id = Integer.parseInt(features_str[0]);
        
		for (int i=0; i < features_str.length-1; i++){
			if (Integer.parseInt(features_str[i+1]) == 1){
                features_probabilities_arr[class_id][i]++;
            }
		}
		num_of_members_in_each_class[class_id]++;
	}

}
