#ifndef _SEVERCONST_H_
#define _SEVERCONST_H_

//html����ģ�崢��


#define INDEX_ROOT_HTML  u8"<a href=\"/register\" >�ҺŹ���</a><br>"\
                           "<a href=\"/doctorschedule\" >ҽ���Ű����</a><br>"\
                           "<a href=\"/prescription\" >ҽ����������</a><br>"\
                           "<a href=\"/operation\" >��������</a><br>"\
                           "<a href=\"/nurseschedule\" >��Sʿ����վ����</a><br>"\
                           "<a href=\"/hospitalin\" >��Ժ�Ǽ�</a><br>"\
                           "<a href=\"/hospitalmanger\" >סԺ����</a><br>"\
                           "<a href=\"/drugin\" >ҩ�﷢��</a><br>"\
                           "<a href=\"/drugout\" >ҩ��ɹ�</a><br>"\
                           "<a href=\"/drugreservation\" >�ֿ����</a><br>"\
                           "<a href=\"web3.html\" >����֧������</a><br>"\
                           "<a href=\"web3.html\" >�����������</a><br>"\
                           "<a href=\"web3.html\" >ҽ������ͳ��</a><br>"\
                           "<a href=\"web3.html\" >����ͳ��</a><br>"\
                           "<a href=\"/login_pwd\" >ע��</a><br>"\
                           "<a href=\"/login\" >��¼</a><br>"

#define  DOCTOR_FORM_HTML     u8"<form action=\"/doctorschedule_delete\" method=\"post\" enctype=\"application/x-www-form-urlencoded\"> "\
                              "<tr>"\
                              "<td><input type = \"text\" name = \"number\" value = \"%s\" readonly></td>"\
                              "<td><input type = \"text\" name = \"name\" value =   \"%s\" readonly></td>"\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"submit\" value = \"ɾ��\"></td>                       "\
                              "</tr>                                                                     "\
                              "</form>                                                                   "


#define NURSE_FORM_HTML    u8"<form action=\"/nurseschedule_delete\" method=\"post\" enctype=\"application/x-www-form-urlencoded\"> "\
                              "<tr>"\
                              "<td><input type = \"text\" name = \"number\" value = \"%s\" readonly></td>"\
                              "<td><input type = \"text\" name = \"name\" value =   \"%s\" readonly></td>"\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"submit\" value = \"ɾ��\"></td>                       "\
                              "</tr>                                                                     "\
                              "</form>                                                                   "


#define DRUGIN_FORM_HTML    u8" <tr>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " </tr>"

#define DRUGOUT_FORM_HTML   u8" <tr>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " </tr>"

#define DRUGNUM_FORM_HTML   u8" <tr>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " <td><input type = \"text\" value = \"%s\" readonly></td>"\
                              " </tr>"


#define HOSPITALMANAGE_FORM_HTML  u8"<form action=\"/hospital_msg\" enctype=\"application/x-www-form-urlencoded\" method=\"post\">"\
                                    "<tr>                                                                                        "\
                                    "<td><input type = \"text\" name=\"floor\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"text\" name=\"department\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"text\" name=\"bed\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"text\" name=\"name\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"text\" name=\"nursename\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"text\" name=\"doctorname\" value = \"%s\" readonly></td>                                    "\
                                    "<td><input type = \"submit\" value =\"��ϸ��Ϣ\"></td>                                       "\
                                    "</tr>                                                                                       "\
                                    "</form>                                                                                     "


#endif // !_SEVERCONST_H_
