# Контекст решения
<!-- Окружение системы (роли, участники, внешние системы) и связи системы с ним. Диаграмма контекста C4 и текстовое описание. 
-->
```plantuml
@startuml 
!include https://github.com/tainella/MAI_Software_architecture/blob/master/context.puml

Person(admin, "Администратор")
Person(moderator, "Модератор")
Person(user, "Пользователь")

System(conference_site, "Сервис доставки", "Быстрее чем Яндекс")

Rel(admin, conference_site, "Просмотр, добавление и редактирование информации о пользователях, доставках и получателях")
Rel(moderator, conference_site, "Модерация контента и пользователей")
Rel(user, conference_site, "Регистрация, просмотр/изменение информации о доставках и получателях")
@enduml
```
## Назначение систем
|Система| Описание|
|-------|---------|
| Сервис доставки | Веб-интерфейс, обеспечивающий оформление доставок различным получателям. Бэкенд сервиса реализован в виде микросервисной архитектуры |

