t <html><head><title>OVERLOAD</title>
t <script language=JavaScript>
t </script><</head>
i pg_header.inc
t <h2 align=center><br>OVERLOAD</h2>
t <p><font size="2">This page allows you to change the overload value</b>.
t  Simply enter the value you want to compare </b>.<br><br>
t <form action=buttons.cgi method=post name=form1>
t <input type=hidden value="umbral" name=pg>
t <table border=0 width=99%><font size="3">
t <tr bgcolor=#aaccff>
t  <th width=40%>Item</th>
t  <th width=60%>Setting</th></tr>
t <td><img src=pabb.gif>Activar Interrupcion:</td>
t <td><select name="ctrl" onchange="submit();">
c f c <option %s>Enable</option><option %s>Disable</option></select></td></tr>
# Here begin data setting which is formatted in HTTP_CGI.C module
t <tr><td><img src=pabb.gif>Umbral</td>
c f 1 <td><input type=text name=lcd1 size=20 maxlength=20 value="%s"></td></tr>
t </font></table>
# Here begin button definitions
t <p align=center>
t <input type=submit name=set value="Send" id="sbm">
t </p></form>
i pg_footer.inc
. End of script must be closed with period. 

