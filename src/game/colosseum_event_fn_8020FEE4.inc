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
    clrlwi r4, r3, 16
    cmplwi r4, 0x59
    ble @L_8020FF28
    li r3, 0x3
    b @L_8020FF4C
@L_8020FF28:
    cmplwi r4, 0x3e
    blt @L_8020FF38
    li r3, 0x2
    b @L_8020FF4C
@L_8020FF38:
    li r0, 0x16
    li r3, -0x1
    subfc r0, r0, r4
    subfze r0, r3
    clrlwi r3, r0, 24
@L_8020FF4C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
