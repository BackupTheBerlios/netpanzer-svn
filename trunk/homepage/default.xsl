<?xml version="1.0"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  
  <xsl:output 
    method="html" 
    indent="yes" 
    doctype-public="-//W3C//DTD HTML 4.01//EN" 
    doctype-system="http://www.w3.org/TR/html4/strict.dtd"
    encoding="ISO-8859-1" />
  
  <xsl:param name="filename"/>
  
  <xsl:template match="node()|@*">
    <xsl:copy><xsl:apply-templates select="@* | node()" /></xsl:copy>
  </xsl:template>

  <xsl:template match="menu">
    <div class="menu">
      <xsl:apply-templates />
    </div>
  </xsl:template>

  <xsl:template match="menu/item">
    <xsl:choose>
      <xsl:when test="concat($filename, '.html')=@file">
        <a class="active" href="{@file}"><xsl:apply-templates /></a>
      </xsl:when>
      <xsl:otherwise>
        <a href="{@file}"><xsl:apply-templates /></a>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="page">
    <html>
      <head>
        <title>netPanzer - An Online Multiplayer Tactical Warfare Game</title>
        <link rel="stylesheet" type="text/css" href="default.css" />
      </head>

      <body>
        <div style="background-color: #394439;  margin: 0px;  padding: 1em 3em 1px 3em;">
          <div style="border-width: thin; background-color:  #DD0000; padding-top: 0px; padding-bottom: 2px; border-style: none;">
            <table cellspacing="0" cellpadding="0" width="100%">
              <tr>
                <td align="left" valign="top"><img src="images/leftedge.png" alt="" width="28" height="28"/></td>
                <td align="center">
                  <h1><img src="images/netpanzer.png" alt="netPanzer" /></h1>
                </td>
                <td align="right" valign="top"><img src="images/rightedge.png" alt="" width="28" height="28" /></td>
              </tr>
            </table>

            <xsl:apply-templates select="document('menu.xml')" />
          </div>

          <div style="padding: .5em 2em .5em 2em; background-color: black; border-style: none solid solid solid; border-width: 2px; border-color: #dd0000;">
            <xsl:apply-templates />
          </div>

          <div style="font-size: smaller; background-color: black; color: red; padding: 5px; margin-top: 1em; margin-bottom: 1em;">
            <table width="100%" cellspacing="0" cellpadding="0">
              <tr>
                <td align="left" style="width: 45%;">Copyright &#169; 1997-1998, PyroSoft Inc.</td>
                <!-- this probably doesn't make sense at the "official" page
                <td align="center">
                  <a href="http://validator.w3.org/check?uri=http%3A%2F%2Fpingus.seul.org%2F%7Egrumbel%2Ftmp%2FnetPanzer%2F{$filename}.html">html</a>,
                  <a href="http://jigsaw.w3.org/css-validator/validator?uri=http%3A%2F%2Fpingus.seul.org%2F%7Egrumbel%2Ftmp%2FnetPanzer%2F{$filename}.html&amp;warning=1&amp;profile=css2">css</a>
                </td>-->
                <td align="right" style="width: 30%;">
                  Hosting:
                  <a href="http://developer.berlios.de">
                  <img
                  src="http://developer.berlios.de/bslogo.php?group_id=1250"
                  align="middle"
                  width="124" height="32" border="0" alt=""/></a>
                </td>
                <td align="right"  style="width: 45%;">Contact: <a
                href="mailto:netpanzer-devel@lists.berlios.de">netPanzer-Devel</a></td>
              </tr>
            </table>
          </div>
        </div>
      </body>
    </html>
  </xsl:template>

  
  <xsl:template match="section">
    <h2><xsl:value-of select="@title" /></h2>
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="subsection">
    <h3><xsl:value-of select="@title" /></h3>
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="subsubsection">
    <h4><xsl:value-of select="@title" /></h4>
    <xsl:apply-templates />
  </xsl:template>

  <xsl:template match="faq-list">
    <ul id="faqtoc">
      <xsl:for-each select="faq/question">
        <li><a href="#faq{generate-id(.)}">
            <xsl:apply-templates/></a></li>
      </xsl:for-each>
    </ul>
    <hr/>
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="faq">
    <p></p>
    <table width="100%"  class="question">
      <colgroup width="60%" />
      <tr><td valign="top">
          <div id="faq{generate-id(question)}">
            <xsl:apply-templates select="question/node()"/>
          </div>
        </td>
        
        <td align="right" valign="top">
          <xsl:if test="@date!=''">
            <small>Last update:<xsl:value-of select="@date"/></small>
          </xsl:if>
          [<small><a href="#faqtoc">Up</a></small>]
        </td>
      </tr>
    </table>

    <p class="answer"><xsl:apply-templates select="answer/node()"/> </p>
  </xsl:template>
  
  <xsl:template match="news">
    <dl class="news">
      <xsl:apply-templates />
    </dl>
  </xsl:template>

  <xsl:template match="news/item">
    <dt><b><xsl:value-of select="@date" /></b></dt>
    <dd><xsl:apply-templates /></dd>
  </xsl:template>

  <xsl:template match="newsbox">
    <div style="float: right; width: 30%; border-style: solid; border-color: black; margin-top: 1em; padding: 2px; border-width: thin;">
      <div style="background-color: #a09c80; color: black; font-family: sans-serif;"><b>News</b></div>
      <dl class="newsbox">
        <xsl:apply-templates />
      </dl>
      <div align="right" style="margin: 0px;">
        <a href="news.html">Older News</a>
      </div>
    </div>
  </xsl:template>


  <xsl:variable name="screenshots_per_row">3</xsl:variable>                                                                                                             

  <xsl:template match="screenshots">
    <table align="center" width="100%" style="margin-top: 1em;">
      <xsl:for-each select="*[position() mod $screenshots_per_row = 0]">
        <tr>
          <xsl:variable name="row_number"><xsl:value-of select="position()"/></xsl:variable>
          <xsl:for-each select="../*[ceiling((position()) div $screenshots_per_row) = $row_number]">
            <xsl:apply-templates select="."/>
          </xsl:for-each>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>

  <xsl:template match="screenshots/item">
    <td valign="middle" align="center" width="33%">
      <a href="images/{@file}.jpg"><img style="margin-bottom: 1em; margin-top 1em;" src="images/{@file}_small.jpg" alt="{@file}" width="200" height="150" /></a>
    </td>
  </xsl:template>

  <xsl:template match="units">
    <div style="text-align: center;">
      <table>
        <xsl:apply-templates />
      </table>
    </div>
  </xsl:template>

  <xsl:template match="units/unit">
    <tr>
      <td valign="middle">
       <div style="text-align: right;">
        <strong><xsl:value-of select="name" /></strong><br />
        <ul style="margin: 0px; list-style-type: square;"><li><xsl:value-of select="description" /></li></ul>
       </div>
      </td>
      <td valign="middle" style="padding: 1em;"><img src="{image}" alt="" title="{name} - {description}" width="70" height="70"/></td>
    </tr>
  </xsl:template>

  <xsl:template match="newsbox/item">
    <dt><b><xsl:value-of select="@date" /></b></dt>
    <dd><xsl:value-of select="." /></dd>
  </xsl:template>

</xsl:stylesheet>
