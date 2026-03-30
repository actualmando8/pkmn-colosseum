    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r31, r3
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80201CAC
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_80201CAC
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80201D48
@L_80201CAC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80201CEC
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80201D1C
@L_80201CEC:
    cmplwi r31, 0x0
    bne @L_80201CFC
    li r3, 0x0
    b @L_80201D10
@L_80201CFC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80201D10:
    mr r4, r30
    bl fn_8012165C
    b @L_80201D70
@L_80201D1C:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80201D38
    li r3, 0x0
    b @L_80201D70
@L_80201D38:
    mr r3, r31
    mr r4, r30
    bl fn_8011A6D4
    b @L_80201D70
@L_80201D48:
    mr r3, r30
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_80201D64
    li r3, 0x0
    b @L_80201D70
@L_80201D64:
    mr r3, r31
    mr r4, r30
    bl fn_8011A6D4
@L_80201D70:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
