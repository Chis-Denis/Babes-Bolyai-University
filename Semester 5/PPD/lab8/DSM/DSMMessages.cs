namespace DSMLib;

public enum MessageType : byte
{
    Write = 0,
    WriteAck = 1,
    WriteCommit = 2,
    Cas = 3,
    CasAck = 4,
    CasCommit = 5,
    CasAbort = 6
}

public class Message
{
    public MessageType Type { get; set; }
    public int VarId { get; set; }
    public int ProcessId { get; set; }
    public int Value { get; set; }
    public int OldValue { get; set; }
    public int NewValue { get; set; }
    public int Timestamp { get; set; }
    public int SenderId { get; set; }
    public int MsgId { get; set; }

    public Message()
    {
        Type = MessageType.Write;
        VarId = 0;
        ProcessId = 0;
        Value = 0;
        OldValue = 0;
        NewValue = 0;
        Timestamp = 0;
        SenderId = 0;
        MsgId = 0;
    }

    public byte[] Serialize()
    {
        // Convert message to byte array for network transmission
        using var ms = new MemoryStream();
        using var writer = new BinaryWriter(ms);
        
        writer.Write((byte)Type);
        writer.Write(VarId);
        writer.Write(ProcessId);
        writer.Write(Value);
        writer.Write(OldValue);
        writer.Write(NewValue);
        writer.Write(Timestamp);
        writer.Write(SenderId);
        writer.Write(MsgId);
        
        return ms.ToArray();
    }

    public static Message Deserialize(byte[] data)
    {
        // Convert byte array back to Message object (inverse of Serialize)
        using var ms = new MemoryStream(data);
        using var reader = new BinaryReader(ms);
        
        var msg = new Message
        {
            Type = (MessageType)reader.ReadByte(), // Read in same order as Serialize
            VarId = reader.ReadInt32(),
            ProcessId = reader.ReadInt32(),
            Value = reader.ReadInt32(),
            OldValue = reader.ReadInt32(),
            NewValue = reader.ReadInt32(),
            Timestamp = reader.ReadInt32(),
            SenderId = reader.ReadInt32(),
            MsgId = reader.ReadInt32()
        };
        
        return msg;
    }
}

