import sys
import os
import re

def main() :
    if len(sys.argv) < 3 :
      exit()

    res_file = open(sys.argv[1], 'r')
    xml_file = open(sys.argv[2], 'w')
    
    res_match_exp = re.compile('\w\w*', re.IGNORECASE)
    
    try:
        xml_file.write('<?xml version="1.0" encoding="gb18030" ?>\n')
        xml_file.write('<ResourceIdMap>\n');
    
        for res_file_line in res_file:
            res_info_list = res_match_exp.findall(res_file_line)
            if len(res_info_list) > 0:
                if res_info_list[0] == 'define' :
                    res_to_xml(xml_file, res_info_list[1], res_info_list[2])
             
        xml_file.write('</ResourceIdMap>\n');
        
    finally:
        res_file.close()
        xml_file.close()
        
def res_to_xml(xml_file, res_name, res_id) :
    xml_format = '    <Resource Name="{}" Id="{}" />\n'
    xml_content = xml_format.format(res_name, res_id)
    xml_file.write(xml_content)
    
if __name__ == '__main__':
    main()