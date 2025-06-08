<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"> 
<xsl:template match="/"> 
  <html> 
  <head> 
    <title>单元测试报告</title>
    <style> 
      body {
        font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
        margin: 20px;
        color: #333;
        background-color: #f8f9fa;
      }
      .header {
        background: linear-gradient(135deg, #2e7d32, #1b5e20); /* 绿色渐变 */
        color: white;
        text-align: center;
        padding: 25px 30px;
        border-radius: 8px;
        margin-bottom: 25px;
        box-shadow: 0 4px 12px rgba(0, 0, 0, 0.2);
      }
      .header h1 {
        font-weight: 700;
        margin-bottom: 8px;
        font-size: 2.4rem;
        text-shadow: 0 2px 4px rgba(0,0,0,0.3);
      }
      .header p {
        opacity: 0.9;
        font-size: 1.1rem;
        text-shadow: 0 1px 2px rgba(0,0,0,0.2);
      }
      table {
        border-collapse: collapse;
        width: 100%;
        margin-top: 15px;
        box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
        background-color: white;
        border-radius: 8px;
        overflow: hidden;
      }
      th, td {
        border: 1px solid #f0f0f0;
        padding: 14px 20px;
        text-align: left;
        vertical-align: top;
      }
      th {
        background-color: #f1f8e9;
        color: #2e7d32;
        font-weight: 600;
        border-bottom: 2px solid #e0e0e0;
      }
      tr:hover {
        background-color: #f8fffb;
      }
      .result-tag {
        display: inline-block;
        padding: 5px 12px;
        border-radius: 14px;
        font-size: 0.85rem;
        font-weight: 600;
        text-align: center;
        min-width: 70px;
      }
      .true .result-tag {
        background-color: #c8e6c9;
        color: #2e7d32;
      }
      .false .result-tag {
        background-color: #ffcdd2;
        color: #d32f2f;
      }
      .summary {
        background-color: white;
        padding: 25px;
        border-radius: 8px;
        box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);
        margin: 20px 0;
      }
      .summary-grid {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
        gap: 18px;
        margin-top: 10px;
      }
      .summary-item {
        background: white;
        padding: 20px 15px;
        border-radius: 8px;
        box-shadow: 0 2px 6px rgba(0, 0, 0, 0.07);
        text-align: center;
        border: 1px solid #e0e0e0;
        transition: all 0.3s ease;
      }
      .summary-item:hover {
        transform: translateY(-3px);
        box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
      }
      .stats-label {
        font-size: 0.95rem;
        color: #616161;
        margin-bottom: 8px;
        font-weight: 500;
      }
      .stats-value {
        font-weight: 700;
        font-size: 2.2rem;
      }
      .success-count { color: #2e7d32; }
      .failure-count { color: #d32f2f; }
      .assert-count { color: #1976d2; }
      .pass-rate { color: #7b1fa2; }
      .footer {
        text-align: center;
        margin-top: 30px;
        color: #757575;
        font-size: 0.9rem;
      }
      .test-suite {
        background-color: #f1f8e9;
        vertical-align: middle;
        font-weight: bold;
        color: #2e7d32;
      }
      .expression {
        font-family: monospace;
        font-size: 0.95em;
      }
      /* 新增分隔线样式 */
      .suite-divider td {
        border-bottom: 1px solid #bdbdbd !important;
        padding: 0 !important;
        height: 0;
      }
      @media (max-width: 768px) {
        .header h1 {
          font-size: 1.8rem;
        }
        th, td {
          padding: 10px 12px;
        }
      }
    </style> 
  </head> 
  <body> 
    <div class="header">
      <h1>单元测试报告</h1>
      <p>Doctest 单元测试框架 | 详细测试结果</p>
    </div>
    
    <!-- 汇总统计 -->
    <div class="summary">
      <h2>📊 测试概览</h2>
      <div class="summary-grid">
        <xsl:variable name="testSuites" select="count(doctest/TestSuite)"/>
        <xsl:variable name="testCases" select="count(//TestCase)"/>
        <xsl:variable name="successCases" select="doctest/OverallResultsTestCases/@successes"/>
        <xsl:variable name="failureCases" select="doctest/OverallResultsTestCases/@failures"/>
        <xsl:variable name="successAsserts" select="doctest/OverallResultsAsserts/@successes"/>
        <xsl:variable name="failureAsserts" select="doctest/OverallResultsAsserts/@failures"/>
        <xsl:variable name="totalAsserts" select="$successAsserts + $failureAsserts"/>
        <xsl:variable name="passRate" select="format-number($successAsserts div $totalAsserts * 100, '0.00')"/>
        
        <div class="summary-item">
          <div class="stats-label">测试套件</div>
          <div class="stats-value"><xsl:value-of select="$testSuites"/></div>
        </div>
        
        <div class="summary-item">
          <div class="stats-label">测试用例</div>
          <div class="stats-value"><xsl:value-of select="$testCases"/></div>
        </div>
        
        <div class="summary-item">
          <div class="stats-label">成功用例</div>
          <div class="stats-value success-count"><xsl:value-of select="$successCases"/></div>
        </div>
        
        <div class="summary-item">
          <div class="stats-label">失败用例</div>
          <div class="stats-value failure-count"><xsl:value-of select="$failureCases"/></div>
        </div>
        
        <div class="summary-item">
          <div class="stats-label">总断言数</div>
          <div class="stats-value assert-count"><xsl:value-of select="$totalAsserts"/></div>
        </div>
        
        <div class="summary-item">
          <div class="stats-label">通过率</div>
          <div class="stats-value pass-rate"><xsl:value-of select="$passRate"/>%</div>
        </div>
      </div>
    </div>
    
    <h2>🔍 详细测试结果</h2>
    <table> 
      <thead>
        <tr> 
          <th>测试套件</th> 
          <th>测试用例</th> 
          <th>表达式</th> 
          <th>测试结果</th> 
        </tr> 
      </thead>
      <tbody>
        <xsl:choose>
          <xsl:when test="count(//TestCase) = 0">
            <tr>
              <td colspan="4" style="text-align:center; padding:30px; background-color:#f9f9f9;">
                ⚠️ 未发现任何测试用例
              </td>
            </tr>
          </xsl:when>
          <xsl:otherwise>
            <xsl:for-each select="doctest/TestSuite"> 
              <xsl:variable name="testSuite" select="@name"/> 
              <xsl:variable name="totalRows" select="count(TestCase/Expression) + count(TestCase[not(Expression)])"/> 
              <xsl:variable name="suitePosition" select="position()"/>
              <xsl:variable name="totalSuites" select="last()"/>
              
              <xsl:for-each select="TestCase"> 
                <xsl:variable name="testCase" select="@name"/> 
                <xsl:choose> 
                  <xsl:when test="Expression"> 
                    <xsl:for-each select="Expression"> 
                      <tr class="{@success}"> 
                        <xsl:if test="position() = 1 and count(../preceding-sibling::TestCase) = 0"> 
                          <td class="test-suite" rowspan="{$totalRows}"> 
                            <xsl:value-of select="$testSuite"/> 
                          </td> 
                        </xsl:if> 
                        <td><xsl:value-of select="$testCase"/></td> 
                        <td class="expression"><xsl:value-of select="Original"/></td> 
                        <td>
                          <span class="result-tag">
                            <xsl:choose>
                              <xsl:when test="@success = 'true'">通过</xsl:when>
                              <xsl:otherwise>失败</xsl:otherwise>
                            </xsl:choose>
                          </span>
                        </td> 
                      </tr> 
                    </xsl:for-each> 
                  </xsl:when> 
                  <xsl:otherwise> 
                    <tr class="true"> 
                      <xsl:if test="count(preceding-sibling::TestCase) = 0"> 
                        <td class="test-suite" rowspan="{$totalRows}"> 
                          <xsl:value-of select="$testSuite"/> 
                        </td> 
                      </xsl:if> 
                      <td><xsl:value-of select="$testCase"/></td> 
                      <td class="expression">N/A</td> 
                      <td>
                        <span class="result-tag">通过</span>
                      </td> 
                    </tr> 
                  </xsl:otherwise> 
                </xsl:choose> 
              </xsl:for-each> 
              
              <!-- 添加测试套件分隔线 -->
              <xsl:if test="$suitePosition != $totalSuites">
                <tr class="suite-divider">
                  <td colspan="4"></td>
                </tr>
              </xsl:if>
            </xsl:for-each>
          </xsl:otherwise>
        </xsl:choose>
      </tbody>
    </table> 
    
    <div class="footer">
      <p>报告生成时间: <script>document.write(new Date().toLocaleString());</script></p>
      <p>问题反馈: jassimxiong@gmail.com</p>
    </div>
  </body> 
  </html> 
</xsl:template> 
</xsl:stylesheet>