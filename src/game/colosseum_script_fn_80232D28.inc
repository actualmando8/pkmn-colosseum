    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r31, r3
    lwz r0, 0x4(r5)
    lwz r30, 0x0(r5)
    rlwinm r28, r0, 0, 16, 31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80232D60
    li r3, 0x1
    b @L_80232FD0
@L_80232D60:
    cmplw r31, r30
    bne @L_80232D70
    li r3, 0x1
    b @L_80232FD0
@L_80232D70:
    cmplwi r28, 0x7f
    bne @L_80232DFC
    mr r3, r30
    mr r4, r31
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80232ECC
    mr r3, r31
    li r4, 0x1d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232DD8
    mr r3, r31
    li r4, 0x1d
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80232DD8
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80232DD8
    cmplw r3, r30
    bne @L_80232DD8
    li r0, 0x1
    b @L_80232DDC
@L_80232DD8:
    li r0, 0x0
@L_80232DDC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232ECC
    mr r3, r31
    li r4, 0x1d
    li r5, 0x0
    bl fn_80201FDC
    b @L_80232ECC
@L_80232DFC:
    mr r3, r31
    li r4, 0x16
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232E48
    mr r3, r31
    li r4, 0x16
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80232E48
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80232E48
    cmplw r3, r30
    bne @L_80232E48
    li r0, 0x1
    b @L_80232E4C
@L_80232E48:
    li r0, 0x0
@L_80232E4C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232E64
    mr r3, r31
    li r4, 0x16
    bl fn_80202810
@L_80232E64:
    mr r3, r31
    li r4, 0x1d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232EB0
    mr r3, r31
    li r4, 0x1d
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80232EB0
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80232EB0
    cmplw r3, r30
    bne @L_80232EB0
    li r0, 0x1
    b @L_80232EB4
@L_80232EB0:
    li r0, 0x0
@L_80232EB4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232ECC
    mr r3, r31
    li r4, 0x1d
    bl fn_80202810
@L_80232ECC:
    mr r3, r31
    li r4, 0xa
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232F18
    mr r3, r31
    li r4, 0xa
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80232F18
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80232F18
    cmplw r3, r30
    bne @L_80232F18
    li r0, 0x1
    b @L_80232F1C
@L_80232F18:
    li r0, 0x0
@L_80232F1C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232F34
    mr r3, r31
    li r4, 0xa
    bl fn_80202810
@L_80232F34:
    mr r3, r31
    li r4, 0xe
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232F80
    mr r3, r31
    li r4, 0xe
    bl fn_80201D84
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80232F80
    mr r4, r29
    bl fn_801F00D0
    cmplwi r3, 0x0
    beq @L_80232F80
    cmplw r3, r30
    bne @L_80232F80
    li r0, 0x1
    b @L_80232F84
@L_80232F80:
    li r0, 0x0
@L_80232F84:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80232F9C
    mr r3, r31
    li r4, 0xe
    bl fn_80202810
@L_80232F9C:
    mr r3, r31
    mr r4, r30
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80232FCC
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_80201340
    mr r3, r31
    mr r4, r30
    bl fn_80203198
@L_80232FCC:
    li r3, 0x1
@L_80232FD0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
