pushd %~dp0

protoc.exe -I=./ --cpp_out=./ ./Enum.proto
protoc.exe -I=./ --cpp_out=./ ./Struct.proto
protoc.exe -I=./ --cpp_out=./ ./Protocol.proto

GenPackets.exe --path=./Protocol.proto --output=ClientPacketHandler --recv=S_ --send=C_
GenPackets.exe --path=./Protocol.proto --output=ServerPacketHandler --recv=C_ --send=S_

IF ERRORLEVEL 1 PAUSE

COPY /Y Enum.pb.h "../../../GameServer"
COPY /Y Enum.pb.cc "../../../GameServer"
COPY /Y Struct.pb.h "../../../GameServer"
COPY /Y Struct.pb.cc "../../../GameServer"
COPY /Y Protocol.pb.h "../../../GameServer"
COPY /Y Protocol.pb.cc "../../../GameServer"
COPY /Y ServerPacketHandler.h "../../../GameServer"

COPY /Y Enum.pb.h "../../../DummyClient"
COPY /Y Enum.pb.cc "../../../DummyClient"
COPY /Y Struct.pb.h "../../../DummyClient"
COPY /Y Struct.pb.cc "../../../DummyClient"
COPY /Y Protocol.pb.h "../../../DummyClient"
COPY /Y Protocol.pb.cc "../../../DummyClient"
COPY /Y ClientPacketHandler.h "../../../DummyClient"

COPY /Y Enum.pb.h "../../../../S1/Source/S1/Network"
COPY /Y Enum.pb.cc "../../../../S1/Source/S1/Network"
COPY /Y Struct.pb.h "../../../../S1/Source/S1/Network"
COPY /Y Struct.pb.cc "../../../../S1/Source/S1/Network"
COPY /Y Protocol.pb.h "../../../../S1/Source/S1/Network"
COPY /Y Protocol.pb.cc "../../../../S1/Source/S1/Network"
COPY /Y ClientPacketHandler.h "../../../../S1/Source/S1"

DEL /Q /F *.pb.h
DEL /Q /F *.pb.cc
DEL /Q /F *.h

PAUSE