import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.MapWritable;

public class MapArrayWritable extends ArrayWritable {

	public MapArrayWritable(MapWritable[] values) {
		super(MapWritable.class, values);
	}

	public MapArrayWritable() {
		super(MapWritable.class);
	}

	public MapArrayWritable(Class valueClass, MapWritable[] values) {
		super(MapWritable.class, values);
	}

	public MapArrayWritable(Class valueClass) {
		super(MapWritable.class);
	}

	public MapArrayWritable(String[] strings) {
		super(strings);
	}

}
