    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r29, r5
    mr r31, r3
    mr r30, r4
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80201A14
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_80201A14
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80201AF4
@L_80201A14:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80201A6C
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80201ACC
@L_80201A6C:
    cmplwi r31, 0x0
    bne @L_80201A7C
    li r31, 0x0
    b @L_80201A94
@L_80201A7C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80201A94:
    cmplwi r30, 0x0
    bne @L_80201AA4
    li r4, 0x0
    b @L_80201ABC
@L_80201AA4:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
@L_80201ABC:
    mr r3, r31
    mr r5, r29
    bl fn_80121484
    b @L_80201B18
@L_80201ACC:
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_80201B18
    mr r3, r31
    mr r4, r30
    mr r5, r29
    bl fn_8011A0A8
    b @L_80201B18
@L_80201AF4:
    mr r3, r29
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    bne @L_80201B18
    mr r3, r31
    mr r4, r30
    mr r5, r29
    bl fn_8011A0A8
@L_80201B18:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
