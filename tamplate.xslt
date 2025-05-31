<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"> 
<xsl:template match="/"> 
  <html> 
  <head> 
    <style> 
      table {  
        border-collapse: collapse;  
        width: 100%; 
      } 
      th, td {  
        border: 1px solid lightgray;  
        padding: 8px;  
        text-align: left; 
      } 
      .false { color: red; } 
      .true { color: green; } 
      .header { 
        background-color: lightgray; 
        font-weight: bold; 
      } 
    </style> 
  </head> 
  <body> 
    <h2>单元测试报告</h2> 
    <h3>问题反馈: jassimxiong@gmail.com</h3> 
    <table> 
      <tr class="header"> 
        <th>测试套件</th> 
        <th>测试用例</th> 
        <th>表达式</th> 
        <th>测试结果</th> 
      </tr> 
      <xsl:for-each select="doctest/TestSuite"> 
        <xsl:variable name="testSuite" select="@name"/> 
        <xsl:for-each select="TestCase"> 
          <xsl:variable name="testCase" select="@name"/> 
          <xsl:choose> 
            <xsl:when test="Expression"> 
              <xsl:for-each select="Expression"> 
                <tr> 
                  <td><xsl:value-of select="$testSuite"/></td> 
                  <td><xsl:value-of select="$testCase"/></td> 
                  <td><xsl:value-of select="Original"/></td> 
                  <td class="{@success}"><xsl:value-of select="@success"/></td> 
                </tr> 
              </xsl:for-each> 
            </xsl:when> 
            <xsl:otherwise> 
              <tr> 
                <td><xsl:value-of select="$testSuite"/></td> 
                <td><xsl:value-of select="$testCase"/></td> 
                <td>N/A</td> 
                <td class="true">true</td> 
              </tr> 
            </xsl:otherwise> 
          </xsl:choose> 
        </xsl:for-each> 
      </xsl:for-each> 
    </table> 
    <h2>汇总:</h2> 
    <p>成功用例总计: <xsl:value-of select="doctest/OverallResultsTestCases/@successes"/></p> 
    <p>失败用例总计: <xsl:value-of select="doctest/OverallResultsTestCases/@failures"/></p> 
    <p>成功断言总计: <xsl:value-of select="doctest/OverallResultsAsserts/@successes"/></p> 
    <p>失败断言总计: <xsl:value-of select="doctest/OverallResultsAsserts/@failures"/></p> 
  </body> 
  </html> 
</xsl:template> 
</xsl:stylesheet>