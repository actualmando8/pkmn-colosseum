    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r3, r4
    li r4, 0x0
    stw r0, 0x14(r1)
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmpwi r0, 0x14
    beq @L_80210548
    bge @L_80210534
    cmpwi r0, 0xa
    beq @L_80210540
    b @L_80210558
@L_80210534:
    cmpwi r0, 0x1e
    beq @L_80210550
    b @L_80210558
@L_80210540:
    li r3, 0x0
    b @L_8021055C
@L_80210548:
    li r3, 0x1
    b @L_8021055C
@L_80210550:
    li r3, 0x2
    b @L_8021055C
@L_80210558:
    li r3, 0x0
@L_8021055C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
