<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" indent="yes"/>

<xsl:template match="/">
<html>
<head>
  <title>Cppcheck 报告</title>
  <style>
    body {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      margin: 20px;
      color: #333;
    }
    .header {
      background-color: #c8e6c9; /* 浅绿色背景 */
      color: #2e7d32; /* 深绿色文字 */
      font-weight: bold;
      text-align: center;
      padding: 10px;
      border-radius: 4px;
      margin-bottom: 20px;
    }
    h1, h2 {
      color: #2e7d32;
      border-bottom: 2px solid #c8e6c9;
      padding-bottom: 8px;
    }
    table {
      border-collapse: collapse;
      width: 100%;
      margin-top: 15px;
      box-shadow: 0 1px 3px rgba(0,0,0,0.1);
    }
    th, td {
      border: 1px solid #e0e0e0;
      padding: 10px 12px;
      text-align: left;
      vertical-align: top;
    }
    th {
      background-color: #f1f8e9;
      font-weight: bold;
    }
    .error {
      background-color: #ffebee; /* 错误红色背景 */
    }
    .warning {
      background-color: #fff8e1; /* 警告黄色背景 */
    }
    .style {
      background-color: #e8f5e9; /* 样式建议绿色背景 */
    }
    .information {
      background-color: #e3f2fd; /* 信息蓝色背景 */
    }
    .severity-tag {
      display: inline-block;
      padding: 3px 8px;
      border-radius: 12px;
      font-size: 0.85em;
      font-weight: bold;
    }
    .error .severity-tag { background-color: #ffcdd2; color: #b71c1c; }
    .warning .severity-tag { background-color: #ffecb3; color: #7d6608; }
    .style .severity-tag { background-color: #c8e6c9; color: #1b5e20; }
    .information .severity-tag { background-color: #bbdefb; color: #0d47a1; }
    .location {
      font-family: monospace;
      font-size: 0.9em;
      color: #555;
    }
    .summary {
      background-color: #f1f8e9;
      padding: 15px;
      border-radius: 5px;
      margin: 20px 0;
    }
    .summary-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 15px;
      margin-top: 10px;
    }
    .summary-item {
      background: white;
      padding: 15px;
      border-radius: 4px;
      box-shadow: 0 1px 2px rgba(0,0,0,0.05);
      text-align: center;
    }
    .stats-label {
      font-size: 0.9em;
      color: #666;
      margin-bottom: 5px;
    }
    .stats-value {
      font-weight: bold;
      font-size: 1.8em;
      color: #2e7d32;
    }
    .error-count { color: #d32f2f; }
    .footer {
      text-align: center;
      margin-top: 30px;
      color: #777;
      font-size: 0.9em;
    }
  </style>
</head>
<body>
  <div class="header">
    <h1>Cppcheck 静态分析报告</h1>
    <p>版本 <xsl:value-of select="results/cppcheck/@version"/> | 报告格式版本 <xsl:value-of select="results/@version"/></p>
  </div>
  
  <!-- 汇总统计 -->
  <div class="summary">
    <h2>📊 问题概览</h2>
    <div class="summary-grid">
      <xsl:variable name="errors" select="count(results/errors/error[@severity='error'])"/>
      <xsl:variable name="warnings" select="count(results/errors/error[@severity='warning'])"/>
      <xsl:variable name="styles" select="count(results/errors/error[@severity='style'])"/>
      <xsl:variable name="infos" select="count(results/errors/error[@severity='information'])"/>
      <xsl:variable name="total" select="count(results/errors/error)"/>
      
      <div class="summary-item">
        <div class="stats-label">问题总数</div>
        <div class="stats-value"><xsl:value-of select="$total"/></div>
      </div>
      
      <div class="summary-item">
        <div class="stats-label">错误</div>
        <div class="stats-value error-count"><xsl:value-of select="$errors"/></div>
      </div>
      
      <div class="summary-item">
        <div class="stats-label">警告</div>
        <div class="stats-value"><xsl:value-of select="$warnings"/></div>
      </div>
      
      <div class="summary-item">
        <div class="stats-label">代码风格</div>
        <div class="stats-value"><xsl:value-of select="$styles"/></div>
      </div>
      
      <div class="summary-item">
        <div class="stats-label">信息</div>
        <div class="stats-value"><xsl:value-of select="$infos"/></div>
      </div>
    </div>
  </div>
  
  <!-- 问题详情表格 -->
  <h2>🔍 详细问题列表</h2>
  <table>
    <thead>
      <tr>
        <th>ID</th>
        <th>严重程度</th>
        <th>消息</th>
        <th>位置</th>
        <th>CWE</th>
      </tr>
    </thead>
    <tbody>
      <xsl:choose>
        <xsl:when test="count(results/errors/error) = 0">
          <tr>
            <td colspan="5" style="text-align:center; padding:30px; background-color:#f9f9f9;">
              🎉 恭喜！未发现任何静态分析问题
            </td>
          </tr>
        </xsl:when>
        <xsl:otherwise>
          <xsl:apply-templates select="results/errors/error"/>
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

<xsl:template match="error">
  <tr class="{@severity}">
    <td><xsl:value-of select="@id"/></td>
    <td>
      <span class="severity-tag"><xsl:value-of select="@severity"/></span>
    </td>
    <td>
      <strong><xsl:value-of select="@msg"/></strong>
      <div style="margin-top:5px; font-size:0.9em; color:#666;">
        <xsl:value-of select="@verbose"/>
      </div>
    </td>
    <td class="location">
      <xsl:choose>
        <xsl:when test="location">
          <div><strong>文件:</strong> <xsl:value-of select="location/@file"/></div>
          <div><strong>行:</strong> <xsl:value-of select="location/@line"/></div>
          <xsl:if test="location/@column">
            <div><strong>列:</strong> <xsl:value-of select="location/@column"/></div>
          </xsl:if>
          <xsl:if test="symbol">
            <div><strong>符号:</strong> <xsl:value-of select="symbol"/></div>
          </xsl:if>
        </xsl:when>
        <xsl:otherwise>全局问题</xsl:otherwise>
      </xsl:choose>
    </td>
    <td>
      <xsl:choose>
        <xsl:when test="@cwe">
          <a href="https://cwe.mitre.org/data/definitions/{@cwe}.html" target="_blank" style="color:#2e7d32; text-decoration:none;">
            CWE-<xsl:value-of select="@cwe"/>
          </a>
        </xsl:when>
        <xsl:otherwise>-</xsl:otherwise>
      </xsl:choose>
    </td>
  </tr>
</xsl:template>

</xsl:stylesheet>
