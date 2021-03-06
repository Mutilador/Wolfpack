unit WpConfiguration;

interface

uses Classes;

type TWpConfiguration = class
  protected
    procedure processOption(Group: String; Name: String; Value: String);

  public
    ImportDefinitions: TStringList;

    constructor Create(Filename: String);
    destructor Destroy; override;
end;

implementation

uses SysUtils, XmlReader, StrLib;

{
  Helper Function
}
function LoadFile( FileName: String ): String;
var
	Input: TStream;
	StringStream: TStringStream;
begin
	Input := TFileStream.Create( FileName, fmOpenRead+fmShareDenyNone );
	StringStream := TStringStream.Create( '' );
  StringStream.CopyFrom( Input, Input.Size );
  Result := StringStream.DataString;
  StringStream.Free;
  Input.Free;
end;

{
  Constructor for the Wp Configuration class.
}
constructor TWpConfiguration.Create(Filename: String);
var
  XmlReader: TXMLStringReader;
  Document, Outmost, Group, Option: TXMLNode;
  i, j: Integer;
  Content, GroupName, OptionName, OptionValue: String;
begin
  ImportDefinitions := TStringList.Create;

  Content := LoadFile(Filename);

  XmlReader := TXMLStringReader.Create(Content);
  Document := XmlReader.ParseDocument;

  // Get outmost node
  Outmost := Document.FindNode('preferences');

  for i := 0 to Outmost.NodeCount - 1 do begin
    Group := Outmost.Nodes[i];

    // We found a group
    if (Group.Name = 'group') and (Group.NodeType = xntElement) then begin
      GroupName := '';
      Group.LookupBasicData('name', GroupName);

      if GroupName = '' then
        continue; // Skip this group without a name.

      // Parse options
      for j := 0 to Group.NodeCount - 1 do begin
        Option := Group.Nodes[j];

        if (Option.Name = 'option') and (Option.NodeType = xntElement) then begin
          OptionName := '';
          Option.LookupBasicData('name', OptionName);

          if OptionName = '' then
            continue; // Skip option without a name.

          Option.LookupBasicData('value', OptionValue);
          processOption(GroupName, OptionName, OptionValue);          
        end;
      end;
    end;
  end;

  Document.Free;
  XmlReader.Free;
end;

{
  Destructor for the configuration class.
}
destructor TWpConfiguration.Destroy;
begin
  ImportDefinitions.Free;
  inherited;
end;

{
  Process a wp configuration node.
}
procedure TWpConfiguration.processOption(Group: String; Name: String; Value: String);
begin
  if (Group = 'General') and (Name = 'Definitions') then begin
    ImportDefinitions.Clear;
    StrLib.Split(Value, ';', ImportDefinitions);
  end;
end;

end.
