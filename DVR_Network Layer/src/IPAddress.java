import java.io.Serializable;
import java.util.Objects;

//Done!
public class IPAddress implements Serializable {

    private Short bytes[];
    private String string;



    public IPAddress(String string) {
        bytes = new Short[4];
        this.string = string;
        String[] temp = string.split("\\.");
        for (int i = 0; i < 4; i++) {
            bytes[i] = Short.parseShort(temp[i]);
        }

    }

    public Short[] getBytes()
    {
        return bytes;
    }

    public String getString()
    {
        return string;
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) {return true;}
        if (obj == null || getClass() != obj.getClass()) {return false;}

        IPAddress point = (IPAddress)obj;

        //if (bytes != point.getBytes()) return false;
        //if (this.string != point.getString()) {return false;}
        if(this.string.equals(point.string)) { return true;}

        return false;
    }
    @Override
    public int hashCode() {
        return Objects.hash(this.string);
    }
    @Override
    public String toString() { return string; }

}
