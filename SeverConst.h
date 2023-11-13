#ifndef _SEVERCONST_H_
#define _SEVERCONST_H_

//html插入模板储存


#define INDEX_ROOT_HTML  u8"<a href=\"/register\" >挂号管理</a><br>"\
                           "<a href=\"/doctorschedule\" >医生排班管理</a><br>"\
                           "<a href=\"/prescription\" >医生处方管理</a><br>"\
                           "<a href=\"/operation\" >手术管理</a><br>"\
                           "<a href=\"/nurseschedule\" >护S士工作站管理</a><br>"\
                           "<a href=\"/hospitalin\" >入院登记</a><br>"\
                           "<a href=\"/hospitalmanger\" >住院管理</a><br>"\
                           "<a href=\"/drugin\" >药物发放</a><br>"\
                           "<a href=\"/drugout\" >药物采购</a><br>"\
                           "<a href=\"/drugreservation\" >仓库管理</a><br>"\
                           "<a href=\"web3.html\" >费用支出结算</a><br>"\
                           "<a href=\"web3.html\" >费用收入结算</a><br>"\
                           "<a href=\"web3.html\" >医疗数据统计</a><br>"\
                           "<a href=\"web3.html\" >财务统计</a><br>"\
                           "<a href=\"/login_pwd\" >注册</a><br>"\
                           "<a href=\"/login\" >登录</a><br>"

#define  DOCTOR_FORM_HTML     u8"<form action=\"/doctorschedule_delete\" method=\"post\" enctype=\"application/x-www-form-urlencoded\"> "\
                              "<tr>"\
                              "<td><input type = \"text\" name = \"number\" value = \"%s\" readonly></td>"\
                              "<td><input type = \"text\" name = \"name\" value =   \"%s\" readonly></td>"\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"text\" value =   \"%s\" readonly></td>                "\
                              "<td><input type = \"submit\" value = \"删除\"></td>                       "\
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
                              "<td><input type = \"submit\" value = \"删除\"></td>                       "\
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
                                    "<td><input type = \"submit\" value =\"详细信息\"></td>                                       "\
                                    "</tr>                                                                                       "\
                                    "</form>                                                                                     "


#endif // !_SEVERCONST_H_
