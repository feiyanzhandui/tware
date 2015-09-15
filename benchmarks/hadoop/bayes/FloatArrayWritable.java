import org.apache.hadoop.io.ArrayWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.Writable;

public class FloatArrayWritable extends ArrayWritable {

	public FloatArrayWritable(Writable[] values) {
		super(FloatWritable.class, values);
	}

	public FloatArrayWritable() {
		super(FloatWritable.class);
	}

	public FloatArrayWritable(Class valueClass, Writable[] values) {
		super(FloatWritable.class, values);
	}

	public FloatArrayWritable(Class valueClass) {
		super(FloatWritable.class);
	}

	public FloatArrayWritable(String[] strings) {
		super(strings);
	}

}
