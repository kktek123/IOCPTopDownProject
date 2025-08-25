import argparse
import jinja2
import ProtoParser
import openpyxl 
import io

def xlsx_to_bytes(filepath):
    """Converts an XLSX file to a byte array."""
    try:
        workbook = openpyxl.load_workbook(filepath)
        # Create a BytesIO object to hold the bytes
        byte_stream = io.BytesIO()
        # Save the workbook to the byte stream in XLSX format
        workbook.save(byte_stream)
        # Get the byte array from the stream
        byte_array = byte_stream.getvalue()
        return byte_array
    except Exception as e:
        print(f"Error converting to bytes: {e}")
        return None


def main():
	# Example usage:
	# file_path = "C:/Unreal porject/MMO/Server/Item.xlsx"
	# excel_bytes = xlsx_to_bytes(file_path)

	# if excel_bytes:
	# 	print(f"Successfully converted to byte array. Length: {len(excel_bytes)}")
	# 	with open("Item.bytes", "wb") as bytes_files:
	# 		bytes_files.write(excel_bytes)
	# You can now use the 'excel_bytes' byte array as needed.
	arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
	arg_parser.add_argument('--path', type=str, default='C:\Unreal porject/Serverproject/Server/Common/protoc-21.12-win64/bin/Protocol.proto', help='proto path')
	arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
	arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
	arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
	args = arg_parser.parse_args()

	parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
	parser.parse_proto(args.path)
	file_loader = jinja2.FileSystemLoader('Templates')
	env = jinja2.Environment(loader=file_loader)

	template = env.get_template('PacketHandler.h')
	output = template.render(parser=parser, output=args.output)

	f = open(args.output+'.h', 'w+')
	f.write(output)
	f.close()

	print(output)
	return

if __name__ == '__main__':
	main()