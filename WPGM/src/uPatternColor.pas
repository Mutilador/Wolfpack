unit uPatternColor;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, GR32_Image, GR32, GR32_Layers;

type
  TfrmPatternColor = class(TForm)
    RadioButton1: TRadioButton;
    RadioButton2: TRadioButton;
    RadioButton3: TRadioButton;
    Button1: TButton;
    Timer1: TTimer;
    ImgView321: TImgView32;
    Image1: TImage;
    Image2: TImage;
    Shape1: TShape;
    procedure Timer1Timer(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  frmPatternColor: TfrmPatternColor;

implementation

{$R *.dfm}

uses UOArt, Main;

procedure TfrmPatternColor.Timer1Timer(Sender: TObject);
var
  Layer: TBitmapLayer;

begin
  Image1.Picture.Assign(Main.Art.GetTIle($50d, 0));
  Image1.Transparent := True;  
  Image1.Picture.Bitmap.TransparentColor := clBtnFace;
  Image1.Picture.Bitmap.Transparent := True;

  ImgView321.Scale := 1.0;
  ImgView321.Centered := True;
  ImgView321.Bitmap.SetSize(66, 66);
  ImgView321.Bitmap.Draw(0, 0, Main.Art.GetTile($50d, 0));
  ImgView321.Bitmap.Draw(22, 22, Main.Art.GetTile($50d, 0));

  Layer := ImgView321.Layers.Add(TBitmapLayer) as TBitmapLayer;
  Layer.Bitmap := Main.Art.GetTile($50d, 0);
  Layer.AlphaHit := True;

  Layer := ImgView321.Layers.Add(TBitmapLayer) as TBitmapLayer;
  Layer.Bitmap := Main.Art.GetTile($50d, 0);
  Layer.AlphaHit := True;

  Layer := ImgView321.Layers.Add(TBitmapLayer) as TBitmapLayer;
  Layer.Bitmap := Main.Art.GetTile($50d, 0);
  Layer.AlphaHit := True;

  Layer := ImgView321.Layers.Add(TBitmapLayer) as TBitmapLayer;
  Layer.Bitmap := Main.Art.GetTile($50d, 0);
  Layer.AlphaHit := True;

{  Image322.Bitmap := Main.Art.GetTile($50d, 0);
  Image323.Bitmap := Main.Art.GetTile($50d, 0); }

  Timer1.Enabled := False;
end;

end.
