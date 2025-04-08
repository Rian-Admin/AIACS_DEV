from django import template

register = template.Library()

@register.filter
def sub(value, arg):
    """두 값의 차이를 계산"""
    try:
        return float(value) - float(arg)
    except (ValueError, TypeError):
        return 0
    
@register.filter
def mul(value, arg):
    """두 값의 곱을 계산"""
    try:
        return float(value) * float(arg)
    except (ValueError, TypeError):
        return 0 